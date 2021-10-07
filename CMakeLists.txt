cmake_minimum_required(VERSION 3.16)
project(BondPricer DESCRPTION "Bond Pricing Library for use with Python")

set(CMAKE_CXX_STANDARD C++20)

add_library(bondlibrary SHARED
    src/*.cpp
)

target_include_directories(bondlibrary PRIVATE include/)

enable_testing()
add_subdirectory(tests)

