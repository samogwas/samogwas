set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  )

add_executable( fltm ${MAIN} )
target_link_libraries( fltm boost_program_options boost_filesystem boost_system boost_regex spl lfltm  )

