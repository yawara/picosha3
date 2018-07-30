cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(picosha3)

add_definitions("-Wall -Wno-unknown-pragmas -std=c++14")

enable_testing()
include(cmake/gtest.cmake)
add_subdirectory(test)

add_subdirectory(example)
