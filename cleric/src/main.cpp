/**

MIT License

Copyright (c) 2017 mpomaranski at gmail

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

#define CATCH_CONFIG_RUNNER
#include <boost/program_options.hpp>
#include <catch/catch.hpp>
#include <easylogging++.h>
#include <future>
#include <iostream>
#include <granada/util/application.h>
#include <props.hpp>

#include "cleric_app.hpp"
#include "network/mdns_responder.hpp"

INITIALIZE_EASYLOGGINGPP

using namespace std;
namespace po = boost::program_options;

void setupLogger(int argc, char *argv[]) {
	START_EASYLOGGINGPP(argc, argv);
}

int runTests() {
	return Catch::Session().run();
}

void exposeProperty(const std::string &propertyName, const std::string &defVal, const po::variables_map& vm) {
	string val;
	if (granada::util::application::GetProperty(propertyName).empty()) {
		val = defVal;
	}
	if (vm.count(propertyName))
	{
		val = vm[propertyName].as< string >();
	}
	granada::util::application::SetProperty(propertyName, val);
	LOG(INFO) << "[main] {" << propertyName << "} {" << granada::util::application::GetProperty(propertyName) << "}";
}

int main(int argc, char *argv[]) {
	int result = -1;
	try {

		setupLogger(argc, argv);

		// Declare the supported options.
		po::options_description desc("Allowed options");
		const auto storageStrategyDescription = std::string() + "storage strategy (default = " + cleric::props::STORAGE_STRATEGY_PROP_DEFAULT + ", available: " + cleric::props::STORAGE_STRATEGY_PROP_MEMORY + ", " + cleric::props::STORAGE_STRATEGY_PROP_HDD + ")";
		const auto storageRootDescription = std::string() + "storage root(current_directory/" + cleric::props::STORAGE_ROOT_PROP_DEFAULT + " if not specified)";
		const auto wwwRootDescription = std::string() + "www root(current_directory/" + cleric::props::WWW_ROOT_PROP_DEFAULT + " if not specified)";

		desc.add_options()
			("help", "produce help message, for logging options see https://github.com/zuhd-org/easyloggingpp")
			("tests-only", "run unit tests, do not start server")
			(cleric::props::WWW_ROOT_PROP_NAME.c_str(), po::value<string>(), wwwRootDescription.c_str())
			(cleric::props::STORAGE_ROOT_PROP_NAME.c_str(), po::value<string>(), storageRootDescription.c_str())
			("prop-file", po::value<string>(), "configuration file (current_dir/cleric.properties if not specified)")
			(cleric::props::STORAGE_STRATEGY_PROP_NAME.c_str(), po::value<string>(), storageStrategyDescription.c_str());

		po::variables_map vm;

		po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
		po::notify(vm);

		if (vm.count("help")) {
			cout << desc << "\n";
			result = 0;
		}
		else if (vm.count("tests-only")) {
			exposeProperty(cleric::props::STORAGE_STRATEGY_PROP_NAME, cleric::props::STORAGE_STRATEGY_PROP_MEMORY, vm);
			result = runTests();
		}
		else {
			// read property file if exists
			string propFile = "cleric.properties";
			if (vm.count("prop-file"))
			{
				propFile = vm["prop-file"].as< string >();
			}
			LOG(INFO) << "[main] {prop-file} {" << propFile << "}";
			granada::util::application::set_property_file(propFile);

			// expose properties to others
			exposeProperty(cleric::props::WWW_ROOT_PROP_NAME, cleric::props::WWW_ROOT_PROP_DEFAULT, vm);
			exposeProperty(cleric::props::STORAGE_ROOT_PROP_NAME, cleric::props::STORAGE_ROOT_PROP_DEFAULT, vm);
			exposeProperty(cleric::props::STORAGE_STRATEGY_PROP_NAME, cleric::props::STORAGE_STRATEGY_PROP_DEFAULT, vm);

			ClericApp app;
			app.go();

			// TODO common name
			vector<unsigned char> ip = { 127, 0, 0, 1 };
			cleric::network::MDNSResponder::getInstance().go(string(THING_NAME), 80, ip);

			cout << "Press RETURN to exit" << endl;
			auto key = cin.get();
			granada::util::time::timer::stopAll();
			cleric::network::MDNSResponder::getInstance().stop();
			result = 0;
		}
	}
	catch (const exception &e) {
		LOG(ERROR) << "[main] {unhandled_exception} {" << e.what() << "}";
		result = 2;

		cout << "Press RETURN to exit" << endl;
		auto key = cin.get();
	}

	granada::util::time::timer::stopAll();
	exit(result);
}
