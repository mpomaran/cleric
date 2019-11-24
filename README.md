Current state
-------------------------------------------------------------------------------------------
This is still work in progress, cleric does not talk to arctic yet, controllers are not
finished, but there is not much left for MVP. I expect the first release soon.

MVP is:
 - possibility for MixBox to store sensor values at the Bridge
 - Bridge is visible by WebThings gateway
 - sensor readings from MixBox are exposed by the Bridge
 - real-life setup works and is accessible trough Mozilla IoT website

Next steps to complete MVP
 - integration tests with Arctic & WebThing gateway
 - healthcheck page

Nice to haves:
 - modification (not only read) for properties
 - figureout zeroconf way for setting the MixBox up
 - better encryption for Arctic - grep for "secret"

Build instructions (Windows 64bit)
-------------------------------------------------------------------------------------------
1. Install VCPKG (https://github.com/Microsoft/vcpkg)
2. Make sure environment variables are set:

VCPKG_HOME to the root of the vcpkg directory (so that %VCPKG_HOME%/scripts/buildsystems/vcpkg.cmake is a path to the existing file)
VCPKG_DEFAULT_TRIPLET to x64-windows

4. Go into VCPKG and grab dependencies:

> vcpkg install boost boost-program-options boost-any boost-asio boost-filesystem boost-functional zlib openssl easyloggingpp picojson msgpack mdnsresponder


Rasperry PI build instructions:
-------------------------------------------------------------------------------------------

Install following libraries
 - OpenSSL (sudo apt install libssl-dev)
 
In order to build the software:
1. Create build directory (eg. build)
2. Configure (cd build && cmake ..) 
	if CMAKE complains about missing libraries set CMAKE_MODULE_PATH, if not you can try: export CMAKE_MODULE_PATH=`find /usr -iname "FindOpenSSL.cmake" -exec dirname {} \;`
3. Build (make)

Precompiled third party dependencies are provided for convinience, you will get the same results if you recompile them yourself.

Third party libraries:

CPP REST SDK
GRANADA
REDIS CLIENT https://github.com/nekipelov/redisclient
BOOST https://www.boost.org
OPEN SSL
ZLIB
EASYLOGGER C++
CATCH2
https://github.com/jevinskie/mDNSResponder
https://www.freshports.org/dns/mdnsd/
hg clone https://bitbucket.org/geekman/tinysvcmdns

Uses MDNSd by originlly written by Jeremie Miller (see cleric/src/network/mdsnd directory for license)
