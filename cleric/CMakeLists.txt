set(GRANADA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/granada")

if (UNIX)
  add_definitions(-Wno-sign-compare -Wno-enum-compare)
endif()

if (WIN32)
  add_definitions(/std:c++14)
endif()

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm") 
set (SSL_LIBS "ssl -lcrypto")
ELSE()
set (SSL_LIBS "libeay32.lib ssleay32.lib")
ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

include_directories(include)

add_executable(cleric-server
  src/business/m2m_message.cpp
  src/business/m2m_message_test.cpp
  src/business/portable/crc.cpp
  src/main.cpp
  src/cleric_app.cpp
  src/business/portable/base64/base64.hpp
  src/business/portable/m2m_portable_utils.hpp
  src/business/portable/base64/base64.cpp
  src/business/portable/m2m_portable_utils.cpp
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

target_link_libraries(cleric-server ${Casablanca_LIBRARIES} ${SSL_LIBS}) 
