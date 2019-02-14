# cleric
Backend service for the IoT devices (intended to cooperate with this one: https://github.com/mpomaran/arctic) based on the Granada framework.

Based on following:
 - Granada (https://github.com/webappsdk/granada)
 - CppRestSdk (https://github.com/Microsoft/cpprestsdk
 - boost libraries
 - msgpack

Current state:
 - compiles under VS 2017 community 
 - uses cmake as a build system for linux portability
 - compiles and runs on Raspberry PI
 - one endpoint with serialisation to disk working

Beware: I did not clear licence stuff yet, files LICENSE and CONTRIBUTE are combined Granada and CppRestSdk files, so please read those.

Cleric is MIT license.
