project(client)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

add_executable(client client.cpp recevice.cpp  handler.cpp  clientStatus.cpp)
target_link_libraries(client pthread)