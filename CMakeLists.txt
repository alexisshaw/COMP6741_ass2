cmake_minimum_required(VERSION 2.8.4)
project(COMP6741_ass1)

find_package(Boost 1.55.0 COMPONENTS graph range program_options)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(COMB_SOURCE_FILES combinations/combination.cpp)
add_executable(COMP6741_ass2_combTest ${COMB_SOURCE_FILES})

set(GEN_SOURCE_FILES gen_c.cpp )
add_executable(COM6741_asst2_gen ${GEN_SOURCE_FILES})
target_link_libraries(COM6741_asst2_gen boost_graph boost_regex boost_program_options)

set(ASS2_SOURCE_FILES progAss2.cpp brute.cpp branch.cpp)
add_executable(COMP6741_ass2_solve ${ASS2_SOURCE_FILES})
target_link_libraries(COMP6741_ass2_solve  boost_graph boost_regex boost_program_options)

FILE (GLOB_RECURSE clion_all_headers ${CMAKE_SOURCE_DIR}/*.h)
ADD_CUSTOM_TARGET(all_clion
    SOURCES ${clion_all_headers}
)