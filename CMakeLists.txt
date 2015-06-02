cmake_minimum_required(VERSION 2.8.12)


add_compile_options(-std=c++11 -fopenmp -mtune=corei7-avx) # 2b safe

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")


project(samogwas)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

### DEPENDENCIES
set(LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies/" )
# boost

set(BOOST_ROOT "${LIB_DIR}/boost_1_55/")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)

set(Boost_INCLUDE_DIRS "${LIB_DIR}/boost_1_55/include/")
set(Boost_LIBRARY_DIRS "${LIB_DIR}/boost_1_55/lib/" )
set(Boost_VERSION "105500" )
message(${Boost_VERSION})

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



