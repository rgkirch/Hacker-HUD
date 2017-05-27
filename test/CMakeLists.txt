cmake_minimum_required(VERSION 3.6)

add_subdirectory(googletest)

set(WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(hhTest displayTest.cpp)
#include_directories(.)
target_link_libraries(hhTest gtest gtest_main gmock)