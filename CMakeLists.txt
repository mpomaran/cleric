project(Cleric)

set (CMAKE_VERBOSE_MAKEFILE ON)

# to disable conflict with boost
#add_definitions(-D_TURN_OFF_PLATFORM_STRING)
set (BOOST_DEBUG FALSE CACHE BOOL "Debug on boost not needed" FORCE)
set (BOOST_USE_MULTITHREADED TRUE CACHE BOOL "MT ON, please" FORCE)

set (THIRD_PARTY_ROOT_DIR "${CMAKE_SOURCE_DIR}/thirdparty")

message (STATUS "System processor: " ${CMAKE_SYSTEM_PROCESSOR})

set (WEBSOCKETPP_DIR "${CMAKE_SOURCE_DIR}/libs/websocket-0.8.1")

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
	message ("Compiling for ARM")

	SET(CXX_FLAGS "-Wno-conversion -fPIC")
	SET(LINKER_FLAGS "")

    #set (BOOST_INCLUDEDIR "${THIRD_PARTY_ROOT_DIR}/arm_boost_1_69_0/include")
    #set (BOOST_LIBRARYDIR "${THIRD_PARTY_ROOT_DIR}/arm_boost_1_69_0/lib")

    #message ("${BOOST_INCLUDEDIR}")

    #include_directories("${THIRD_PARTY_ROOT_DIR}/arm_boost_1_69_0/include")
    #link_directories("${THIRD_PARTY_ROOT_DIR}/arm_boost_1_69_0/lib")
	set(WERROR OFF CACHE BOOL "Do not treat Warnings as Errors.")

	include (FindOpenSSL)

ELSE()
	message ("Compiling for Windows")
#	set(WERROR ON CACHE BOOL "Treat Warnings as Errors.")
#	set (BOOST_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/win_boost_1_66_0")
#	set (BOOST_INCLUDEDIR "${CMAKE_SOURCE_DIR}/thirdparty/win_boost_1_66_0/include")
#	set (BOOST_LIBRARYDIR "${CMAKE_SOURCE_DIR}/thirdparty/win_boost_1_66_0/lib")
#	include_directories("${CMAKE_SOURCE_DIR}/thirdparty/win_boost_1_66_0/include")
#	link_directories("${CMAKE_SOURCE_DIR}/thirdparty/win_boost_1_66_0/lib")

#	set (OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/thirdparty/win_openssl-1.0.2n")
#	link_directories("${CMAKE_SOURCE_DIR}/thirdparty/win_openssl-1.0.2n/lib")
#	include_directories("${CMAKE_SOURCE_DIR}/thirdparty/win_openssl-1.0.2n/include")
ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}" )

#set (ZLIB_LIBRARY "${CMAKE_SOURCE_DIR}/thirdparty/zlib-1.2.11/lib/zlibstaticd.lib")
#set (ZLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/zlib-1.2.11/include")

set (CPPREST_EXCLUDE_WEBSOCKETS ON CACHE BOOL "Enable websockets" FORCE)

include_directories("${CMAKE_SOURCE_DIR}/thirdparty/redisclient-0.6.0/include")

#include_directories("${CMAKE_SOURCE_DIR}/thirdparty/msgpack-c-cpp-3.1.1/include")

#include_directories("${CMAKE_SOURCE_DIR}/thirdparty/picojson-1.3.0/")

set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 3.1)
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW) # use MACOSX_RPATH
endif()
if(UNIX)
  project(cpprestsdk C CXX)
else()
  project(cpprestsdk CXX)
endif()

set(CPPREST_VERSION_MAJOR 2)
set(CPPREST_VERSION_MINOR 10)
set(CPPREST_VERSION_REVISION 9)

enable_testing()

set(WERROR ON CACHE BOOL "Treat Warnings as Errors.")
# set(CPPREST_EXCLUDE_WEBSOCKETS OFF CACHE BOOL "Exclude websockets functionality.")
set(CPPREST_EXCLUDE_COMPRESSION OFF CACHE BOOL "Exclude compression functionality.")
set(CPPREST_EXCLUDE_BROTLI ON CACHE BOOL "Exclude Brotli compression functionality.")
set(CPPREST_EXPORT_DIR cpprestsdk CACHE STRING "Directory to install CMake config files.")
set(CPPREST_INSTALL_HEADERS ON CACHE BOOL "Install header files.")
set(CPPREST_INSTALL ON CACHE BOOL "Add install commands.")
# set(BUILD_TESTS OFF CACHE BOOL "Do not build tests")
set(BUILD_SAMPLES OFF CACHE BOOL "Do not build samples")
if(IOS OR ANDROID)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
else()
  set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")
endif()

if(IOS OR ANDROID OR WINDOWS_STORE OR WINDOWS_PHONE)
  set(BUILD_TESTS OFF CACHE BOOL "Build tests.")
  set(BUILD_SAMPLES OFF CACHE BOOL "Build sample applications.")
else()
  set(BUILD_TESTS ON CACHE BOOL "Build tests.")
  set(BUILD_SAMPLES ON CACHE BOOL "Build sample applications.")
endif()

if(WIN32)
  set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Default filename postfix for libraries under configuration DEBUG")
else()
  set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Default filename postfix for libraries under configuration DEBUG")
endif()

if(WIN32)
  set(TOOLSET)
  if(CMAKE_VS_PLATFORM_TOOLSET)
    string(REGEX REPLACE "^v" "" TOOLSET "${CMAKE_VS_PLATFORM_TOOLSET}")
  endif()
  set(CPPREST_ABI_TAG "${TOOLSET}_${CPPREST_VERSION_MAJOR}_${CPPREST_VERSION_MINOR}" CACHE STRING "Postfix tag for the cpprest abi")
else()
  set(CPPREST_ABI_TAG "" CACHE STRING "Postfix tag for the cpprest abi")
endif()

if(ANDROID)
  set(Boost_USE_STATIC_LIBS ON CACHE BOOL "Link against boost statically.")
else()
  set(Boost_USE_STATIC_LIBS OFF CACHE BOOL "Link against boost statically.")
endif()

include(cmake/cpprest_find_boost.cmake)
include(cmake/cpprest_find_zlib.cmake)
include(cmake/cpprest_find_openssl.cmake)
include(cmake/cpprest_find_websocketpp.cmake)
include(cmake/cpprest_find_brotli.cmake)
include(CheckIncludeFiles)
include(GNUInstallDirs)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  add_compile_options(-pthread)
endif()
if(CMAKE_THREAD_LIBS_INIT)
  link_libraries(${CMAKE_THREAD_LIBS_INIT})
endif()

# Internal component selection logic. This allows us to avoid duplicating platform logic in multiple places.
if(CPPREST_EXCLUDE_WEBSOCKETS)
  set(CPPREST_WEBSOCKETS_IMPL none CACHE STRING "Internal use.")
endif()

if(NOT WIN32)
  CHECK_INCLUDE_FILES(xlocale.h HAVE_XLOCALE_H)
endif()

if(APPLE) # Note: also iOS
  set(CPPREST_PPLX_IMPL apple CACHE STRING "Internal use.")
  set(CPPREST_WEBSOCKETS_IMPL wspp CACHE STRING "Internal use.")
  set(CPPREST_FILEIO_IMPL posix CACHE STRING "Internal use.")
  set(CPPREST_HTTP_CLIENT_IMPL asio CACHE STRING "Internal use.")
  set(CPPREST_HTTP_LISTENER_IMPL asio CACHE STRING "Internal use.")
elseif(UNIX AND NOT APPLE) # Note: also android
  set(CPPREST_PPLX_IMPL linux CACHE STRING "Internal use.")
  set(CPPREST_WEBSOCKETS_IMPL wspp CACHE STRING "Internal use.")
  set(CPPREST_FILEIO_IMPL posix CACHE STRING "Internal use.")
  set(CPPREST_HTTP_CLIENT_IMPL asio CACHE STRING "Internal use.")
  set(CPPREST_HTTP_LISTENER_IMPL asio CACHE STRING "Internal use.")
elseif(WINDOWS_PHONE OR WINDOWS_STORE)
  set(CPPREST_PPLX_IMPL winrt CACHE STRING "Internal use.")
  set(CPPREST_WEBSOCKETS_IMPL winrt CACHE STRING "Internal use.")
  set(CPPREST_FILEIO_IMPL winrt CACHE STRING "Internal use.")
  set(CPPREST_HTTP_CLIENT_IMPL winrt CACHE STRING "Internal use.")
  set(CPPREST_HTTP_LISTENER_IMPL none CACHE STRING "Internal use.")
elseif(WIN32)
  set(CPPREST_PPLX_IMPL win CACHE STRING "Internal use.")
  set(CPPREST_WEBSOCKETS_IMPL wspp CACHE STRING "Internal use.")
  set(CPPREST_FILEIO_IMPL win32 CACHE STRING "Internal use.")
  set(CPPREST_HTTP_CLIENT_IMPL winhttp CACHE STRING "Internal use.")
  set(CPPREST_HTTP_LISTENER_IMPL httpsys CACHE STRING "Internal use.")
else()
  message(FATAL_ERROR "Unknown platform. Cannot determine appropriate feature implementations.")
endif()

set(WARNINGS)
set(ANDROID_LIBS)

# Platform (not compiler) specific settings
if(ANDROID)
  # These are used in the shared library case
  set(ANDROID_LIBS atomic dl)
elseif(UNIX) # This includes OSX
elseif(WIN32)
  add_definitions(-DUNICODE -D_UNICODE -DWIN32 -D_SCL_SECURE_NO_WARNINGS)
  if(CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
    add_definitions(-D_WIN32_WINNT=0x0A00)
  else()
    add_definitions(-D_WIN32_WINNT=0x0600)
  endif()

  if(NOT BUILD_SHARED_LIBS)
    # This causes cmake to not link the test libraries separately, but instead hold onto their object files.
    set(TEST_LIBRARY_TARGET_TYPE OBJECT)
  endif()

  add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
else()
  message(FATAL_ERROR "-- Unsupported Build Platform.")
endif()

# Compiler (not platform) specific settings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR IOS)
  message("-- Setting clang options")

  if(ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-pedantic -Wno-attributes -Wno-pointer-arith")
  elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(WARNINGS -Wall -Wextra -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls)
    set(LINUX_SUPPRESSIONS -Wno-overloaded-virtual -Wno-sign-conversion -Wno-deprecated -Wno-unknown-pragmas -Wno-reorder -Wno-char-subscripts -Wno-switch -Wno-unused-parameter -Wno-unused-variable -Wno-deprecated -Wno-unused-value -Wno-unknown-warning-option -Wno-return-type-c-linkage -Wno-unused-function -Wno-sign-compare -Wno-shorten-64-to-32 -Wno-unused-local-typedefs)
    set(WARNINGS ${WARNINGS} ${LINUX_SUPPRESSIONS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage -Wno-unneeded-internal-declaration")
  else()
    set(WARNINGS -Wall -Wextra -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls)
    set(OSX_SUPPRESSIONS -Wno-overloaded-virtual -Wno-sign-conversion -Wno-deprecated -Wno-unknown-pragmas -Wno-reorder -Wno-char-subscripts -Wno-switch -Wno-unused-parameter -Wno-unused-variable -Wno-deprecated -Wno-unused-value -Wno-unknown-warning-option -Wno-return-type-c-linkage -Wno-unused-function -Wno-sign-compare -Wno-shorten-64-to-32 -Wno-unused-local-typedefs)
    set(WARNINGS ${WARNINGS} ${OSX_SUPPRESSIONS})

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wno-return-type-c-linkage -Wno-unneeded-internal-declaration")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-strict-aliasing")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  message("-- Setting gcc options")

  set(WARNINGS -Wall -Wextra -Wunused-parameter -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls -Wunreachable-code)
  set(LD_FLAGS "${LD_FLAGS} -Wl,-z,defs")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-strict-aliasing")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -D_GLIBCXX_USE_SCHED_YIELD -D_GLIBCXX_USE_NANOSLEEP")
  endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  message("-- Setting msvc options")
  set(WARNINGS)
  set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4264")
  add_compile_options(/bigobj)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MP")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP")

  if (WINDOWS_STORE OR WINDOWS_PHONE)
    add_compile_options(/ZW)
  endif()
else()
  message("-- Unknown compiler, success is doubtful.")
  message("CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
endif()

# Reconfigure final output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries)

# These settings can be used by the test targets
set(Casablanca_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(Casablanca_LIBRARY cpprest)
set(Casablanca_LIBRARIES cpprest)
get_directory_property(PARENT_DIR PARENT_DIRECTORY)
if(NOT PARENT_DIR STREQUAL "")
  set(Casablanca_LIBRARIES ${Casablanca_LIBRARIES} PARENT_SCOPE)
endif()

# Finally, the tests all use the same style declaration to build themselves, so we use a function
function(add_casablanca_test NAME SOURCES_VAR)
  add_library(${NAME} ${TEST_LIBRARY_TARGET_TYPE} ${${SOURCES_VAR}})
  message("-- Added test library ${NAME}")
  if(TEST_LIBRARY_TARGET_TYPE STREQUAL "OBJECT")
    foreach(_dep cpprest common_utilities unittestpp)
      target_include_directories(${NAME} PRIVATE $<TARGET_PROPERTY:${_dep},INTERFACE_INCLUDE_DIRECTORIES>)
      target_compile_definitions(${NAME} PRIVATE $<TARGET_PROPERTY:${_dep},INTERFACE_COMPILE_DEFINITIONS>)
    endforeach()
  else()
    target_link_libraries(${NAME} PRIVATE
      cpprest
      common_utilities
      unittestpp
      ${ANDROID_LIBS}
    )
    if (BUILD_SHARED_LIBS)
      add_test(NAME ${NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND test_runner $<TARGET_FILE_NAME:${NAME}>
      )
    endif()
  endif()
endfunction()

add_subdirectory(src)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()


# MPP I disabled samples (to be removed in the future) as I do not need them anymore
#if(BUILD_SAMPLES)
#  add_subdirectory(samples)
#  add_subdirectory(samples/granada)
#endif()

add_subdirectory(cleric)
