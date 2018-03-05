cmake_minimum_required(VERSION 3.0)

project(Communication C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("http/include")
include_directories("process/include")
include_directories("tcp/include")

add_subdirectory(process)
add_subdirectory(http)
add_subdirectory(tcp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(video-server "src/video-server.cpp")
set_target_properties(video-server PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(video-server "process")
target_link_libraries(video-server "tcp")
target_link_libraries(video-server ${OpenCV_LIBS})

add_executable(video-client "src/video-client.cpp")
set_target_properties(video-client PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(video-client "process")
target_link_libraries(video-client "tcp")
target_link_libraries(video-client ${OpenCV_LIBS})
