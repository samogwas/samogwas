
add_compile_options(-std=c++11) # 2b safe

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall " )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall --wrap=memcpy -fPIC -mtune=corei7-avx")


file(GLOB_RECURSE SOURCE "${PROJECT_SOURCE_DIR}/core/src/*.cpp" )


add_library( lfltm ${SOURCE}  )
target_link_libraries( lfltm  spl  )
