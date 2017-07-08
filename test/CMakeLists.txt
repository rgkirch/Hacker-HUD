cmake_minimum_required(VERSION 3.6)
project(gtestproj)

include_directories("../pio/src")
include_directories("../pio/lib/ArduinoJson")
include_directories(".")

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_FLAGS “${CMAKE_CXX_FLAGS} -std=gnu++11“)
#set(CMAKE_CXX_FLAGS “-std=gnu++11“)
set(CMAKE_FLAGS “--gmock_verbose=info“)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(googletest)
#include(CMakeListsPrivate.txt)

#set(WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(hhTest ../pio/src/makeGetRequest.cpp displayTest.cpp makeGetRequestTest.cpp myConcreteConnectionTest.cpp gridTest.cpp transformTest.cpp chronoTest.cpp)
#add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(hhTest gtest gtest_main gmock)
target_include_directories(hhTest PRIVATE arduinoStubs)