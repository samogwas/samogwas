add_compile_options(-std=c++11 -fopenmp -mtune=corei7-avx) # 2b safe

ENABLE_TESTING()

include(BoostTestTargets)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

set(LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies/" )

include_directories( 
  "${Boost_INCLUDE_DIR}"
  "${PROJECT_SOURCE_DIR}/core/include" 
  "${CMAKE_CURRENT_SOURCE_DIR}" 
)


file(GLOB_RECURSE TESTS ./* )
file(GLOB_RECURSE SOURCE "${PROJECT_SOURCE_DIR}/core/src/*.cpp" )

link_directories("${LIB_DIR}/probt_spl_2_3_99/lib/")

add_boost_test( samogwas_test_suite
    SOURCES ${TESTS} ${SOURCE}
    LIBRARIES spl boost_system boost_filesystem	boost_log boost_thread rt
    TESTS
    Test_Clustering_Test/test_case1
    Test_Statistics_Test/test_case1
)

