set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_compile_options( -std=c++11  -fPIC -O5 -fopenmp -mtune=corei7-avx ) 
file(GLOB MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  )


include_directories( "${PROJECT_SOURCE_DIR}/apps/utils" )
MESSAGE( "${PROJECT_SOURCE_DIR}/apps/utils" )



add_executable( fltm ${MAIN} )
target_link_libraries( fltm spl lfltm  boost_program_options boost_filesystem boost_system boost_regex boost_log boost_thread rt pthread )




