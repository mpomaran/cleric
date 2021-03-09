# cleric

Cleric is multiplatform software designed to present <https://github.com/mpomaran/arctic> device as a WebThing  <https://webthings.io/>. It is written in C++. Raspberry build instructions show how to run the Cleric on the same physical box as the WebThings gateway.

## Compilation instructions

This is a CMake project, so it's fairly portable. Below are instructions for several popular platforms. Windows path is the fastest way to get the software running (should take less than an hour), Raspberry build will take the whole night.

### Windows

1. Install VS Community 2017 (not tested with newer versions, but should also work)
2. Run Developer Command Prompt for VS 2017
3. Go to your working directory
4. Clone VCPKG (<https://github.com/Microsoft/vcpkg>)

```bash
> git clone https://github.com/microsoft/vcpkg.git
```

5. Make sure environment variables are set:

```bash
> set VCPKG_HOME # to the root of the vcpkg directory (so that %VCPKG_HOME%/scripts/buildsystems/vcpkg.cmake is a path to the existing file)
> set VCPKG_DEFAULT_TRIPLET=x64-windows
```

5. Go into VCPKG and grab dependencies:

```bash
> cd %VCPKG_HOME%
> bootstrap-vcpkg.bat
> vcpkg install boost boost-program-options boost-any boost-asio boost-filesystem boost-functional zlib openssl easyloggingpp picojson msgpack
> vcpkg integrate install # Copy output to the notepad
```

6. Open the CMake project (from the main directory) or manually generate project files:
```bash
> cd {cleric directory}
> mkdir build
> cd build
> cmake cmake .. -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows # or whatever 
your output from the integrate install was 
```

### Rasperry PI

After completing the steps provided below you'll have a running WebThings Gateway, with Cleric as a bridge between the Gateway and <https://github.com/mpomaran/arctic>.

1. Go to <https://webthings.io/docs/gateway-getting-started-guide.html> and set up WebThings Gateway (the below was tested with version 1.0).
2. Enable SSH (<https://iot.mozilla.org/docs/gateway-user-guide.html>).
3. Login into the Raspberry and compile dependencies

```bash
pi@gateway:~ $ passwd     # change the default password if not changed previously
pi@gateway:~ $ git clone https://github.com/mpomaran/cleric.git # clone the repo
pi@gateway:~ $ git clone https://github.com/microsoft/vcpkg # clone the repo
```

4. For Raspberry PI 3 you need to do some additional tweak in order to prevent memory issues.

- Increase swap partition size, otherwise Boost won't compile (instructions here <https://pimylifeup.com/raspberry-pi-swap-file/>). I bumped it to 2048:

```bash
pi@gateway:~ $ sudo dphys-swapfile swapoff
pi@gateway:~ $ sudo vim /etc/dphys-swapfile
pi@gateway:~ $ sudo dphys-swapfile setup
pi@gateway:~ $ sudo dphys-swapfile swapon
```

- You may want to limit number of concurrent compilation during vcpkg bootstrp (see <https://github.com/microsoft/vcpkg/issues/14534> or <https://stackoverflow.com/questions/51557585/limit-cpu-cores-for-ndkbuild-with-cmake-and-ninja>). To do so you need to modify vcpkg/scripts/bootstrap.sh with additional parameters (in my case I had to replace line 301), like this:

```bash
(cd "$buildDir" && CXX="$CXX" "$cmakeExe" "$srcDir" -DCMAKE_BUILD_TYPE=Release -G "Ninja" "-DCMAKE_MAKE_PROGRAM=$ninjaExe" "-DCMAKE_JOB_POOL_COMPILE:STRING=compile" "-DCMAKE_JOB_POOL_LINK:STRING=link" '-DCMAKE_JOB_POOLS:STRING=compile=1;link=1'  "-DBUILD_TESTING=$vcpkgBuildTests" "-DVCPKG_DEVELOPMENT_WARNINGS=OFF" "-DVCPKG_ALLOW_APPLE_CLANG=$vcpkgAllowAppleClang") || exit 1 
```

5. Compile dependencies

pi@gateway:~ $ nice bash cleric/setup_dependencies_raspberry.sh # download dependencies and compile; Warning! it will take a couple of hours, you might want to use "screen" to run this task in the background
```bash
```

6. Compile Cleric

```bash
pi@gateway:~ $ cd cleric && mkdir build && cd build
pi@gateway:~ $ cmake .. -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
pi@gateway:~ $ nice make # to build Cleric

# Note: if CMake complains about missing OpenSSL libraries set CMAKE_MODULE_PATH, or exec followiog before issuing the make command:
export CMAKE_MODULE_PATH=`find /usr -iname "FindOpenSSL.cmake" -exec dirname {} \;`
```

Below points, while valid, need to be expanded:

7. Modify properties (cleric/cleric/test_data/cleric.properties), expose the correct port (corresponding to Arctic), setup DNS, so that Arctic can find your gateway. Look for the cleric.properties in test directory, set bindinding according to comments, set port to the exposed one (default is 85) 

8. Copy properties to the directory where the cleric binary is located, Run the cleric, turn on Arctic, wait for it to send the firt message

- you can doublecheck if the server is running by browsing <http://gateway.local:85> on your web browser
- if you run on defaults (default secrets, etc) you can simulate Arctic by putting something like <http://gateway.local:85/m2m/1/00000000A643C98690000000066334873A687ABBBF27B175F> in your browser, it should insert some fake data

9. Add Cleric as a WebThing

- log in into the gatway trough WWW (as described in <https://webthings.io/docs/gateway-getting-started-guide.html> )
- click "+" in the bottom right corner
- click "Add by URL..."
- enter following URL in the "Enter web thing URL...": http://127.0.0.1:85/things/mixbox-bridge
- click "Submit"
- fill the name, icon and click "Save", then "Done"

### Google Cloud Platform builds

Scripts for Jenkins build can be found in the ./devops directory. For details on how to setup builds in GCP you can look here: <https://cloud.google.com/solutions/using-jenkins-for-distributed-builds-on-compute-engine>.
Example job configuration is stored in devops/config.xml (no DSL yet). Replace "TODO" with proper repo location and credentials.

## Things to do

- update links in www (cleric/www/index.html)
- integration tests with Arctic
- add more to the instruction
- write installation scripts to run Cleric as a daemon in Webthings environment
- clean up the repo, make it smaller
- modification (not only read) of properties, perhaps support for uBasic commands
- figure out zeroconf way for setting the MixBox up
- storage - use off the shelf solution instead of the plain file
- better encryption for Arctic - grep for "secret"
- healthcheck page

## Third-party libraries used

- CPP REST SDK
- GRANADA
- REDIS CLIENT <https://github.com/nekipelov/redisclient>
- BOOST <https://www.boost.org>
- OPEN SSL
- ZLIB
- EASYLOGGER C++
- CATCH2
- <https://github.com/jevinskie/mDNSResponder>
- <https://www.freshports.org/dns/mdnsd/>
- hg clone <https://bitbucket.org/geekman/tinysvcmdns>
- MDNSd by originally written by Jeremie Miller (see cleric/src/network/mdsnd directory for license)
