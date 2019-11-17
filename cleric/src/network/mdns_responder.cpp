/**

MIT License

Copyright (c) 2019 mpomaranski at gmail

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#if defined(WINVER) && !defined(__MINGW32__) 
#include <winsock2.h>
#endif

#include "mdns_responder.hpp"
#include <thread>
#include <easylogging++.h>

#if defined(__MINGW32__) && (!defined(WINVER) || WINVER < 0x501)
/* Assume the target is newer than Windows XP */
# undef WINVER
# undef _WIN32_WINDOWS
# undef _WIN32_WINNT
# define WINVER _WIN32_WINNT_VISTA
# define _WIN32_WINDOWS _WIN32_WINNT_VISTA
# define _WIN32_WINNT _WIN32_WINNT_VISTA
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <sys/types.h>

#ifdef _WIN32

# include <winsock2.h>
# include <ws2tcpip.h>

#include <sys/types.h>

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static size_t write(int fd, const void *buf, size_t count)
{
	return send(fd, (const char *)buf, (int)count, 0);
}

static int my_inet_pton(int af, const char *src, void *dst)
{
	struct sockaddr_storage ss;
	int size = sizeof(ss);
	char src_copy[INET6_ADDRSTRLEN + 1];

	ZeroMemory(&ss, sizeof(ss));
	/* stupid non-const API */
	strncpy(src_copy, src, INET6_ADDRSTRLEN + 1);
	src_copy[INET6_ADDRSTRLEN] = 0;

	if (WSAStringToAddress((LPWSTR)src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
		switch (af) {
		case AF_INET:
			*(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
			return 1;
		case AF_INET6:
			*(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
			return 1;
		}
	}
	return 0;
}

#define INET_PTON my_inet_pton
#else
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <sys/ioctl.h>
#define INET_PTON inet_pton
#endif

#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
# define CLOSESOCKET(S) closesocket((SOCKET)S)
# define ssize_t int
#else
# include <unistd.h> // read, write, close
# define CLOSESOCKET(S) close(S)
#endif

#include "libmdnsd/mdnsd.h"
#include "libmdnsd/sdtxt.h"

namespace cleric {
namespace network {

MDNSResponder MDNSResponder::instance;

static bool should_stop = false;

static mdns_daemon_t *_d;
static int daemon_socket = 0;

void conflict(char *name, int type, void *arg)
{
	LOG(ERROR) << "conflicting name detected " << name << " for type " << type;
}

static void socket_set_nonblocking(int sockfd) {
#ifdef _WIN32
	u_long iMode = 1;
	ioctlsocket(sockfd, FIONBIO, &iMode);
#else
	int opts = fcntl(sockfd, F_GETFL);
	fcntl(sockfd, F_SETFL, opts | O_NONBLOCK);
#endif
}

/* Create multicast 224.0.0.251:5353 socket */
int msock(void)
{
	int s, flag = 1, ittl = 255;
	struct sockaddr_in in;
	struct ip_mreq mc;
	char ttl = 255; // send to any reachable net, not only the subnet

	memset(&in, 0, sizeof(in));
	in.sin_family = AF_INET;
	in.sin_port = htons(5353);
	in.sin_addr.s_addr = 0;

	if ((s = (int)socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return 0;

#ifdef SO_REUSEPORT
	setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (char *)&flag, sizeof(flag));
#endif
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));
	if (bind(s, (struct sockaddr *)&in, sizeof(in))) {
		CLOSESOCKET(s);
		return 0;
	}

	mc.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
	mc.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&mc, sizeof(mc));
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl));
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ittl, sizeof(ittl));

	socket_set_nonblocking(s);

	return s;
}

void record_received(const struct resource* r, void* data) {
	// NOP for now
	// TODO test if setting up this callback is needed for mdnsd lib
}

void responder(::std::string serviceName, unsigned short int port, const ::std::vector<unsigned char> ip_) noexcept {
	mdns_daemon_t *d;
	mdns_record_t *r;
	struct in_addr ip;
	fd_set fds;
	int s;
	char hlocal[256], nlocal[256];
	unsigned char *packet;
	int len = 0;
	xht_t *h;
	char *path = NULL;

	_d = d = mdnsd_new(QCLASS_IN, 1000);
	if ((s = msock()) == 0) {
		LOG(ERROR) << "can't create socket: " << strerror(errno);
		throw std::runtime_error("Cannot create socket");
	}

#if defined(WINVER) && !defined(__MINGW32__) 

	ip.S_un.S_un_b.s_b1 = ip_[0];
	ip.S_un.S_un_b.s_b2 = ip_[1];
	ip.S_un.S_un_b.s_b3 = ip_[2];
	ip.S_un.S_un_b.s_b4 = ip_[3];

#else
	{
		union {
			unsigned char b[4];
			uint32_t i;
		} addr;

		addr.b[0] = ip_[0];
		addr.b[1] = ip_[1];
		addr.b[2] = ip_[2];
		addr.b[3] = ip_[3];
		ip.s_addr = addr.i;
	}
#endif

	mdnsd_register_receive_callback(d, record_received, NULL);

	sprintf(hlocal, "%s._webthing._tcp.local.", serviceName.c_str());
	sprintf(nlocal, "webthing-%s.local.", serviceName.c_str());

	// Announce that we have a _http._tcp service
	r = mdnsd_shared(d, "_services._dns-sd._udp.local.", QTYPE_PTR, 120);
	mdnsd_set_host(d, r, "_webthing._tcp.local.");

	r = mdnsd_shared(d, "_webthing._tcp.local.", QTYPE_PTR, 120);
	mdnsd_set_host(d, r, hlocal);
	r = mdnsd_unique(d, hlocal, QTYPE_SRV, 600, conflict, 0);
	mdnsd_set_srv(d, r, 0, 0, port, nlocal);
	r = mdnsd_unique(d, nlocal, QTYPE_A, 600, conflict, 0);
	mdnsd_set_raw(d, r, (char *)&ip.s_addr, 4);
	r = mdnsd_unique(d, hlocal, QTYPE_TXT, 600, conflict, 0);
	h = xht_new(11);
	if (path && strlen(path))
		xht_set(h, "path", path);
	packet = sd2txt(h, &len);
	xht_free(h);
	mdnsd_set_raw(d, r, (char *)packet, len);
	MDNSD_free(packet);

	// example for getting a previously published record:
	{
		mdns_record_t *get_r = mdnsd_get_published(d, "_webthing._tcp.local.");
		while (get_r) {
			const mdns_answer_t *data = mdnsd_record_data(get_r);
			LOG(INFO) << "Found record of type " << data->type;
			get_r = mdnsd_record_next(get_r);
		}
	}

	LOG(WARNING) << "[mdnsresponder] {started}";

	struct timeval next_sleep;
	next_sleep.tv_sec = 0;
	next_sleep.tv_usec = 0;

	while (should_stop == false) {

		FD_ZERO(&fds);
		FD_SET(s, &fds);
		select(s + 1, &fds, 0, 0, &next_sleep);

		if (should_stop)
			break;

		{
			unsigned short retVal = mdnsd_step(d, s, FD_ISSET(s, &fds), true, &next_sleep);
			if (retVal == 1) {
				LOG(ERROR) << "can't read from socket " << errno << ": " << strerror(errno);
				break;
			}
			else if (retVal == 2) {
				LOG(ERROR) << "can't write to socket: " << strerror(errno);
				break;
			}
		}

		if (should_stop)
			break;
		::std::this_thread::yield();
	}

	mdnsd_shutdown(d);
	mdnsd_free(d);
}


MDNSResponder& MDNSResponder::getInstance() {
	return MDNSResponder::instance;
}

MDNSResponder::~MDNSResponder() {
	stop();
}

void MDNSResponder::go(const ::std::string & service, unsigned short int port, const ::std::vector<unsigned char> & ip)
{
	if (ip.size() != 4) {
		throw ::std::runtime_error("Wrong IP parameter");
	}

	if (t) {
		throw ::std::runtime_error("Responder already running");
	}

	should_stop = false;

	t = ::std::make_unique<::std::thread>(responder, service, port, ip);
}

void MDNSResponder::stop()
{
	if (t) {
		should_stop = true;
		mdnsd_shutdown(_d);
		write(daemon_socket, "\0", 1);
		t->join();
		t = nullptr;
		LOG(TRACE) << "[mdnsresponder] {stopped}";
	}
}

MDNSResponder::MDNSResponder() : t(nullptr)
{}

}
}
