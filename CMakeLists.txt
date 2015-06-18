cmake_minimum_required(VERSION 2.8.12)


add_compile_options(-std=c++11 -mtune=corei7-avx) # 2b safe

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

project(samogwas)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#openMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

### DEPENDENCIES
set(LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies" )
# boost
set(Boost_NO_SYSTEM_PATHS ON)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(BOOST_ROOT "${LIB_DIR}/boost_1_55")

find_package(Boost COMPONENTS log log_setup system regex filesystem program_options thread REQUIRED)

#probt
set(ProBT_INCLUDE_DIR "${LIB_DIR}/probt_spl_2_3_99/include/")
set(ProBT_LIBRARY_DIR "${LIB_DIR}/probt_spl_2_3_99/lib/" )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${Boost_INCLUDE_DIRS}" "${ProBT_INCLUDE_DIR}"  
  "${PROJECT_SOURCE_DIR}/core/include" )

link_directories("${LIB_DIR}/probt_spl_2_3_99/lib/" "${LIB_DIR}/" "${Boost_INCLUDE_DIRS}" "${Boost_LIBRARY_DIRS}")

add_subdirectory(core)

add_subdirectory(apps)

# test related
include(CTest)
add_subdirectory(tests)




