set(GRANADA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/granada")

if (UNIX)
  add_definitions(-Wno-sign-compare -Wno-enum-compare)
endif()

include_directories(include)

add_executable(cleric-server
  src/main.cpp
  ${GRANADA_SOURCE_DIR}/defaults.cpp
  ${GRANADA_SOURCE_DIR}/functions.cpp
  ${GRANADA_SOURCE_DIR}/util/file.cpp
  ${GRANADA_SOURCE_DIR}/util/application.cpp
  ${GRANADA_SOURCE_DIR}/http/parser.cpp
  ${GRANADA_SOURCE_DIR}/http/http_msg.cpp
  ${GRANADA_SOURCE_DIR}/http/oauth2/oauth2.cpp
  ${GRANADA_SOURCE_DIR}/http/oauth2/map_oauth2.cpp
  ${GRANADA_SOURCE_DIR}/crypto/nonce_generator.cpp
  ${GRANADA_SOURCE_DIR}/cache/web_resource_cache.cpp
  ${GRANADA_SOURCE_DIR}/cache/shared_map_cache_driver.cpp
  ${GRANADA_SOURCE_DIR}/http/session/session.cpp
  ${GRANADA_SOURCE_DIR}/http/session/map_session.cpp
  ${GRANADA_SOURCE_DIR}/http/controller/browser_controller.cpp
  ${GRANADA_SOURCE_DIR}/http/controller/oauth2_controller.cpp
  )

target_link_libraries(cleric-server ${Casablanca_LIBRARIES} libeay32 ssleay32 )
