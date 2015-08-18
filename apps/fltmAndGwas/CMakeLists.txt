set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_compile_options( -std=c++11 -fPIC -fopenmp -mtune=corei7-avx ) 

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O5 -Wall")

file(GLOB MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  )
include_directories( "${PROJECT_SOURCE_DIR}/apps/utils" )
MESSAGE( "${PROJECT_SOURCE_DIR}/apps/utils" )

add_executable( fltmAndGwas ${MAIN} )
target_link_libraries( fltmAndGwas spl lfltm  boost_program_options boost_filesystem boost_system boost_regex boost_log boost_thread rt pthread boinc_api boinc )




