#! /bin/bash

assert_result () {
	RESULT=$?
	if [ $RESULT -ne 0 ]
	then
		echo "Error, bailing out"
		exit $RESULT
	fi
}
#sudo apt install -y libssl-dev zip
#assert_result
echo "Building CMake"
#wget https://github.com/Kitware/CMake/releases/download/v3.19.6/cmake-3.19.6.tar.gz
#assert_result
#tar xf cmake-3.19.6.tar.gz
#assert_result
WD=`pwd`
cd cmake-3.19.6
#assert_result
#./bootstrap -- -DCMAKE_BUILD_TYPE:STRING=Release
#assert_result
#make
#assert_result
#sudo make install
#assert_result
#sudo apt-get install -y ninja-build
#assert_result
echo "Going back to $WD"
cd ${WD}
assert_result
echo "Building vcpkg"
#assert_result
#./vcpkg/bootstrap-vcpkg.sh -useSystemBinaries
#assert_result
echo "Building dependencies"
export  VCPKG_FORCE_SYSTEM_BINARIES=1
./vcpkg/vcpkg install boost boost-program-options boost-any boost-asio boost-filesystem boost-functional boost-random boost-filesystem boost-regex zlib easyloggingpp picojson msgpack
assert_result
#./vcpkg/vcpkg install openssl is it needed? see sudp apt install
sudo VCPKG_FORCE_SYSTEM_BINARIES=1 ./vcpkg/vcpkg integrate install
assert_result
echo "Completed successfully"
