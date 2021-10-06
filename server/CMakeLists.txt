project(server)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

add_executable(server  server.cpp receive.cpp serverStatus.cpp data.h handler.cpp handler.h util.cpp util.h)

target_link_libraries(server pthread)