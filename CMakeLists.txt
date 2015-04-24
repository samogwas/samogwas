cmake_minimum_required(VERSION 2.8.12)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O5 -fPIC -fopenmp -mtune=corei7-avx") 

project(samogwas)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_compile_options(-std=c++0x -O5 -fPIC -fopenmp -mtune=corei7-avx) # 2b safe

### DEPENDENCIES
# boost
find_package(Boost 1.54 REQUIRED)

#probt
set(LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies/" )
set(ProBT_INCLUDE_DIR "${LIB_DIR}/probt_spl_2_3_99/include/")
set(ProBT_LIBRARY_DIR "${LIB_DIR}/probt_spl_2_3_99/lib/" )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" ${Boost_INCLUDE_DIRS} "${ProBT_INCLUDE_DIR}")
link_directories( ${ProBT_LIBRARY_DIR})
# test related
include(CTest)
add_subdirectory(tests)





