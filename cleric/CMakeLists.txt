cmake_policy(SET CMP0074 OLD)

set(GRANADA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/granada")

FIND_PACKAGE(Boost 1.69 COMPONENTS program_options filesystem REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)

message("Found boost in:'${Boost_INCLUDE_DIRS}' ")

find_path(EASYLOGGINGPP_INCLUDE easylogging++.h PATHS "${THIRD_PARTY_ROOT_DIR}/easyloggingpp-master/src")
if("${EASYLOGGINGPP_INCLUDE}" STREQUAL "EASYLOGGINGPP_INCLUDE-NOTFOUND")
	message(SEND_ERROR "Could not found easylogging++.h, please specify CMAKE_PREFIX_PATH")
else()
	message("Found easylogging++.h in:'${EASYLOGGINGPP_INCLUDE}' ")
endif()

find_path(PICOJSON_INCLUDE picojson.h PATH_SUFFIXES picojson)
if("${PICOJSON_INCLUDE}" STREQUAL "PICOJSON_INCLUDE-NOTFOUND")
	message(SEND_ERROR "Could not found picojson.h, please specify CMAKE_PREFIX_PATH")
else()
	message("Found picojson.h in:'${PICOJSON_INCLUDE}' ")
endif()

#find_package(msgpack CONFIG REQUIRED)

#The package msgpack:x64-windows provides CMake targets:
#
#    find_package(msgpack CONFIG REQUIRED)
#    target_link_libraries(main PRIVATE msgpackc msgpackc msgpackc-cxx)

if (UNIX)
  add_definitions(-Wno-sign-compare -Wno-enum-compare)
endif()

if (WIN32)
  add_definitions(/std:c++14)
endif()

add_definitions(-DELPP_THREAD_SAFE -DELPP_NO_DEFAULT_LOG_FILE)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIRS})
include_directories(${MSGPACK_INCLUDE_DIRS})
include_directories(${EASYLOGGINGPP_INCLUDE})
include_directories(${PICOJSON_INCLUDE})

include_directories(include)

add_executable(cleric-server
  ${EASYLOGGINGPP_INCLUDE}/easylogging++.cc
  src/main.cpp
  src/cleric_app.cpp
  src/props.cpp
  src/business/m2m_message.cpp
  src/business/m2m_message_test.cpp
  src/business/portable/crc.cpp
  src/business/portable/base64/base64.hpp
  src/business/portable/m2m_portable_utils.hpp
  src/business/portable/base64/base64.cpp
  src/business/portable/m2m_portable_utils.cpp
  src/data/box.cpp
  src/data/box_test.cpp
  src/data/box_server.cpp
  src/data/box_server_test.cpp
  src/data/box_server_locator.cpp
  src/data/box_server_locator_test.cpp
  src/data/hdd_box_serialization_strategy.cpp
  src/data/hdd_box_serialization_strategy_test.cpp
  src/controller/m2m_controller.cpp
  src/controller/m2m_controller_test.cpp
  src/controller/boxes_controller.cpp
  src/controller/boxes_controller_test.cpp
  src/controller/things_controller.cpp
  src/controller/things_controller_test.cpp
  src/controller/m2m_controller_tester.cpp
  src/network/libmdnsd/1035.c
  src/network/libmdnsd/mdnsd.c
  src/network/libmdnsd/sdtxt.c
  src/network/libmdnsd/xht.c
  src/network/host_info.cpp
  src/network/mdns_responder.cpp
  ${GRANADA_SOURCE_DIR}/cache/redis_cache_driver.cpp
  ${GRANADA_SOURCE_DIR}/cache/shared_map_cache_driver.cpp
  ${GRANADA_SOURCE_DIR}/cache/web_resource_cache.cpp
  ${GRANADA_SOURCE_DIR}/crypto/nonce_generator.cpp
  ${GRANADA_SOURCE_DIR}/defaults.cpp
  ${GRANADA_SOURCE_DIR}/functions.cpp
  ${GRANADA_SOURCE_DIR}/http/controller/browser_controller.cpp
  ${GRANADA_SOURCE_DIR}/http/controller/oauth2_controller.cpp
  ${GRANADA_SOURCE_DIR}/http/http_msg.cpp
  ${GRANADA_SOURCE_DIR}/http/oauth2/map_oauth2.cpp
  ${GRANADA_SOURCE_DIR}/http/oauth2/oauth2.cpp
  ${GRANADA_SOURCE_DIR}/http/oauth2/redis_oauth2.cpp
  ${GRANADA_SOURCE_DIR}/http/parser.cpp
  ${GRANADA_SOURCE_DIR}/http/session/map_session.cpp
  ${GRANADA_SOURCE_DIR}/http/session/redis_session.cpp
  ${GRANADA_SOURCE_DIR}/http/session/session.cpp
  ${GRANADA_SOURCE_DIR}/util/application.cpp
  ${GRANADA_SOURCE_DIR}/util/file.cpp
  )

target_link_libraries(cleric-server ${Casablanca_LIBRARIES} ${OPENSSL_LIBS}
	${Boost_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto) 

if(WIN32 OR MINGW)
  target_link_libraries(cleric-server wsock32 ws2_32)
endif()

#set(UNIT_TEST cleric-server)
#add_custom_command(
#     TARGET ${UNIT_TEST}
#     COMMENT "Run tests"
#     POST_BUILD 
#     COMMAND ${UNIT_TEST} --tests-only
#)
