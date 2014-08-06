#=============================================================================#
# Author: Travis McLane                                                       #
# Date:   08.06.2014                                                          #
#                                                                             #
# Description:                                                                #
#                                                                             #
#=============================================================================#
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)
project(fermware)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries)

add_subdirectory(src)
