cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(googletest)
add_executable(gtesttest gTestProj.cpp)
include_directories(../lib)
target_link_libraries(gtesttest vfd gtest gtest_main gmock)
