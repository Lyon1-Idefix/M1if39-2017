cmake_minimum_required (VERSION 3.0)

project (Ws C CXX)

# source files

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")

# includes and libraries

include_directories(".")
include_directories("include")
include_directories("src")

option(USE_STANDALONE_ASIO "set ON to use standalone Asio instead of Boost.Asio" OFF)
option(BUILD_TESTING "set ON to build library tests" OFF)

if(NOT MSVC)
    add_compile_options(-std=c++11 -Wall -Wextra -Wsign-conversion)
else()
    add_compile_options(/W1)
endif()

add_library(simple-websocket-server INTERFACE)

target_include_directories(simple-websocket-server INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Threads REQUIRED)
target_link_libraries(simple-websocket-server INTERFACE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(simple-websocket-server INTERFACE simple-web)

# TODO 2020 when Debian Jessie LTS ends:
# Remove Boost system, thread, regex components; use Boost::<component> aliases; remove Boost target_include_directories
if(USE_STANDALONE_ASIO)
    target_compile_definitions(simple-websocket-server INTERFACE USE_STANDALONE_ASIO)
    include(CheckIncludeFileCXX)
    CHECK_INCLUDE_FILE_CXX(asio.hpp HAVE_ASIO)
    if(NOT HAVE_ASIO)
        message(FATAL_ERROR "Standalone Asio not found")
    endif()
else()
    find_package(Boost 1.54.0 COMPONENTS system thread coroutine context REQUIRED)
    target_link_libraries(simple-websocket-server INTERFACE ${Boost_LIBRARIES})
    target_include_directories(simple-websocket-server INTERFACE ${Boost_INCLUDE_DIR})
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        target_compile_definitions(simple-websocket-server INTERFACE USE_BOOST_REGEX)
        find_package(Boost 1.54.0 COMPONENTS regex REQUIRED)
        target_link_libraries(simple-websocket-server INTERFACE ${Boost_LIBRARIES})
        target_include_directories(simple-websocket-server INTERFACE ${Boost_INCLUDE_DIR})
    endif()
endif()
if(WIN32)
    target_link_libraries(simple-websocket-server INTERFACE ws2_32 wsock32)
endif()

if(APPLE)
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()
find_package(OpenSSL REQUIRED)
target_link_libraries(simple-websocket-server INTERFACE ${OPENSSL_LIBRARIES})
target_include_directories(simple-websocket-server INTERFACE ${OPENSSL_INCLUDE_DIR})

    add_executable(ws-server tests/ws-server.cpp)
    target_link_libraries(ws-server simple-web)
    target_link_libraries(ws-server simple-websocket-server)
    find_package(Boost 1.54.0 COMPONENTS system thread filesystem REQUIRED)
    target_link_libraries(ws-server ${Boost_LIBRARIES})
    target_include_directories(ws-server PRIVATE ${Boost_INCLUDE_DIR})

    add_executable(ws-client tests/ws-client.cpp)
    target_link_libraries(ws-client simple-web)
    target_link_libraries(ws-client simple-websocket-server)
    find_package(Boost 1.54.0 COMPONENTS system thread filesystem REQUIRED)
    target_link_libraries(ws-client ${Boost_LIBRARIES})
    target_include_directories(ws-client PRIVATE ${Boost_INCLUDE_DIR})
