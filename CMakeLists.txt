cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O5 -fPIC -fopenmp -mtune=corei7-avx") 

project(samogwas)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Boost 1.55 REQUIRED)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}" ${Boost_INCLUDE_DIRS})

# test related
include(CTest)
add_subdirectory(tests)





