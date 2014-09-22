cmake_minimum_required(VERSION 2.8.4)
project(COMP6741_ass1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(COMB_SOURCE_FILES combinations/combination.cpp)
add_executable(COMP6741_comb_test ${COMB_SOURCE_FILES})

set(ASS2_SOURCE_FILES progAss2.cpp)
add_executable(COMP6741_comb_ass2 $(ASS2_SOURCE_FILES))

FILE (GLOB_RECURSE clion_all_headers ${CMAKE_SOURCE_DIR}/*.h)
ADD_CUSTOM_TARGET(all_clion
    SOURCES ${clion_all_headers}
)