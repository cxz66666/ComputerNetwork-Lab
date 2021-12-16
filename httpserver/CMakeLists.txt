cmake_minimum_required(VERSION 3.16)
project(httpserver)

set(CMAKE_CXX_STANDARD 20)

add_executable(httpserver httpserver.cpp main.cpp httpserver.h config.h)
target_link_libraries(httpserver pthread)