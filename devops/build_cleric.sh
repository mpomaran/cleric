#! /bin/sh

cd Cleric
rm -rf build
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=/home/ubuntu/vcpkg/scripts/buildsystems/vcpkg.cmake ..
make -j4
cd Binaries


result=0
./cleric-server --tests-only
o=$?
if [ result != $o ]; then result=$o; fi

exit $result