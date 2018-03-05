cmake_minimum_required(VERSION 3.0)

project(Tcp C CXX)

# source files

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")

# includes and libraries

include_directories(".")
include_directories("include")
include_directories("src")

# build libraries

add_library(tcp SHARED "${LIB_SOURCES}")
set_target_properties(tcp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(tcp "process")

# build executables

add_executable(chat-server "tests/chat-server.cpp")
set_target_properties(chat-server PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(chat-server "process")
target_link_libraries(chat-server "tcp")

add_executable(chat-client "tests/chat-client.cpp")
set_target_properties(chat-client PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(chat-client "process")
target_link_libraries(chat-client "tcp")
