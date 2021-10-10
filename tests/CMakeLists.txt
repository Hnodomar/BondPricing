cmake_minimum_required(VERSION 3.16)

find_package(Catch2 2 REQUIRED)

add_executable(unit_tests unit_tests.cpp ${BondLibrary_src})
target_include_directories(unit_tests PRIVATE ../include)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2)
target_compile_definitions(unit_tests PRIVATE __TESTS__)

include(CTest)
include(Catch)
catch_discover_tests(unit_tests)
