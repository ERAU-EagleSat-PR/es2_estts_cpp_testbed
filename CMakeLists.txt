cmake_minimum_required(VERSION 3.20)
project(es2_estts_cpp_testbed)

set(CMAKE_CXX_STANDARD 14)

add_executable(es2_estts_cpp_testbed main.cpp socket_handler.cpp)
