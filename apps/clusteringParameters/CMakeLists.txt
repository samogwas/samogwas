set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_compile_options( -std=c++11  -fPIC -O5 -fopenmp -mtune=corei7-avx ) 
file(GLOB MAINO ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  )

add_executable( clusteringParameters ${MAINO} )
target_link_libraries( clusteringParameters spl lfltm  boost_program_options boost_filesystem boost_system boost_regex boost_log boost_thread rt pthread )

