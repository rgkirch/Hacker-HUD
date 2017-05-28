cmake_minimum_required(VERSION 3.6)


include_directories("../Arduino/hardware/arduino/avr/libraries/SoftwareSerial/src")
include_directories("../Arduino/hardware/esp8266com/esp8266/cores/esp8266")
include_directories("../pio/src")

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_FLAGS “${CMAKE_CXX_FLAGS} -std=gnu++11“)
#set(CMAKE_CXX_FLAGS “-std=gnu++11“)
set(CMAKE_CXX_STANDARD 11)


add_subdirectory(googletest)
#include(CMakeListsPrivate.txt)

#set(WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(hhTest displayTest.cpp)
#add_executable(${PROJECT_NAME} ${SRC_LIST})
#include_directories(.)
target_link_libraries(hhTest gtest gtest_main gmock)