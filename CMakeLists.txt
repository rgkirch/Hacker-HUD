cmake_minimum_required(VERSION 3.2)
project(Hacker-HUD)

include(CMakeListsPrivate.txt)
#SET(CMAKE_C_COMPILER "C:/Users/Richie/.platformio/packages/toolchain-xtensa/bin/xtensa-lx106-elf-gcc.exe")
#SET(CMAKE_CXX_COMPILER "C:/Users/Richie/.platformio/packages/toolchain-xtensa/bin/xtensa-lx106-elf-g++.exe")

add_custom_target(
        monitor
        COMMAND ${PLATFORMIO_CMD} device monitor -b 115200 -p com9
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        list
        COMMAND ${PLATFORMIO_CMD} device list
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

#add_custom_target(
#    PLATFORMIO_BUILD ALL
#    COMMAND ${PLATFORMIO_CMD} -f -c clion run
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)
#
#add_custom_target(
#    PLATFORMIO_UPLOAD ALL
#    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)
#
#add_custom_target(
#    PLATFORMIO_CLEAN ALL
#    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target clean
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)
#
#add_custom_target(
#    PLATFORMIO_TEST ALL
#    COMMAND ${PLATFORMIO_CMD} -f -c clion test
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)
#
#add_custom_target(
#    PLATFORMIO_PROGRAM ALL
#    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target program
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)
#
#add_custom_target(
#    PLATFORMIO_UPLOADFS ALL
#    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)
#
#add_custom_target(
#    PLATFORMIO_UPDATE_ALL ALL
#    COMMAND ${PLATFORMIO_CMD} -f -c clion update
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)
#
#add_custom_target(
#    PLATFORMIO_REBUILD_PROJECT_INDEX ALL
#    COMMAND ${PLATFORMIO_CMD} -f -c clion init --ide clion
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

add_executable(${PROJECT_NAME} ${SRC_LIST})
add_subdirectory(test)
add_subdirectory(lib)
