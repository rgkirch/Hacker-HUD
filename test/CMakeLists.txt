cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(googletest)
#add_subdirectory(lib/display)
add_executable(gtesttest lib/display/gTestProj.cpp)
include_directories(../pio/lib/display)
target_link_libraries(gtesttest display gtest gtest_main gmock)
