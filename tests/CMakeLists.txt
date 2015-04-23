#add_subdirectory(clustering)
#add_subdirectory(statistics)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

set(LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies/" )
set(Boost_INCLUDE_DIR "${LIB_DIR}/boost_1_55_0/target/include/")
set(Boost_LIBRARY_DIR "${LIB_DIR}/boost_1_55_0/target/lib/")

include_directories( 
  "${Boost_INCLUDE_DIR}"
)

link_directories(${Boost_LIBRARY_DIR} )

file(
    GLOB_RECURSE
    TESTS
    ./*
)


#file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  )

add_executable( test_samogwas ${TESTS} )
target_link_libraries( test_samogwas boost_unit_test_framework   )

