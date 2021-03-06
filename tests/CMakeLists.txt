add_compile_options(-std=c++11 -mtune=corei7-avx -pthread -static-libstdc++ -static-libgcc ) # 2b safe

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

add_boost_test( samogwas_test_suite
    SOURCES ${TESTS} ${SOURCE}
    LIBRARIES spl  boost_system boost_filesystem boost_program_options  boost_regex boost_log boost_thread rt pthread
    TESTS
    Test_Clustering_Test/test_case1
    Test_Statistics_Test/test_case1
)

