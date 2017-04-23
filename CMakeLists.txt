cmake_minimum_required(VERSION 3.2)
project(Hacker-HUD)
add_definitions(-std=gnu++11)
set(SOURCE_FILES src/main.cpp)

#include(CMakeListsPrivate.txt)
add_custom_target(
        upload
        COMMAND pio run -t upload
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        monitor
        COMMAND pio device monitor -b 115200
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
