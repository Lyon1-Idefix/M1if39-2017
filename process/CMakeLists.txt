cmake_minimum_required(VERSION 3.0)

project(Process C CXX)

# source files

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")

# includes and libraries

include_directories(".")
include_directories("include")
include_directories("src")

# build libraries

add_library(process SHARED "${LIB_SOURCES}")
set_target_properties(process PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(process "pthread")
