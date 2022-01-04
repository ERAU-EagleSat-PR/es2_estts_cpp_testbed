#
# EagleSat 2 Ground Station Telemetry & Telecommunications System Testbed Build Configuration.
# CMake minimum version must be at least 3.4
# CSDK assumes a minimum language standard of C99.
#

cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(es2_estts_cpp_testbed)

add_subdirectory(src)
add_subdirectory(lib/spdlog)
add_subdirectory(test/testbed_config_test)