cmake_minimum_required(VERSION 3.16)
project(BondPricer DESCRPTION "Bond Pricing Library for use with Python")

set(CMAKE_CXX_STANDARD C++20)
set(CMAKE_SHARED_MODULE_PREFIX "")
set(BondLibrary_inc include/)
set(BondLibrary_src src/*.cpp)

find_package(PythonLibs 2.7 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)

add_library(bondlibrary MODULE
    ${BondLibrary_src}
)
target_include_directories(bondlibrary PRIVATE 
    ${BondLibrary_inc} 
)
target_link_libraries(bondlibrary PRIVATE
    ${Boost_LIBRARIES} 
    ${PYTHON_LIBRARIES}
)

enable_testing()
add_subdirectory(tests)

