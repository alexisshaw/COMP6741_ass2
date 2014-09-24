cmake_minimum_required(VERSION 2.8.4)
project(COMP6741_ass1)

find_package(Boost 1.55.0 COMPONENTS graph range program_options)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(COMB_SOURCE_FILES combinations/combination.cpp)
add_executable(COMP6741_comb_test ${COMB_SOURCE_FILES})


set(ASS2_SOURCE_FILES progAss2.cpp brute.cpp branch.cpp kernelisation/graph_reduction.hpp)
add_executable(COMP6741_comb_ass2 ${ASS2_SOURCE_FILES})
target_link_libraries(COMP6741_comb_ass2  boost_graph boost_regex boost_program_options)

FILE (GLOB_RECURSE clion_all_headers ${CMAKE_SOURCE_DIR}/*.h)
ADD_CUSTOM_TARGET(all_clion
    SOURCES ${clion_all_headers}
)