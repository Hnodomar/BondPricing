cmake_minimum_required(VERSION 3.16)

find_package(Catch2 2 REQUIRED)

add_executable(unit_tests unit_tests.cpp ${BondLibrary_src})
target_include_directories(unit_tests PRIVATE ${BondLibrary_inc})
target_link_libraries(unit_tests PRIVATE Catch2::Catch2)

catch_discover_tests(unit_tests)
