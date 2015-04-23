
ENABLE_TESTING()

include(BoostTestTargets)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

set(LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies/" )

include_directories( 
  "${Boost_INCLUDE_DIR}"
)


file(GLOB_RECURSE TESTS ./* )

#add_executable( test_samogwas ${TESTS} )
#target_link_libraries( test_samogwas boost_unit_test_framework )

add_boost_test( samogwas_test_suite
    SOURCES ${TESTS}
    TESTS
     Test_Clustering_Test/test_case1
     Test_Statistics_Test/test_case1
)

