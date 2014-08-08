#=============================================================================#
# Author: Travis McLane                                                       #
# Date:   08.06.2014                                                          #
#                                                                             #
# Description:                                                                #
#                                                                             #
#=============================================================================#

# Note: This is above the CMAKE_TOOLCHAIN_FILE to allow the HOST to be detected
#       not the cross-compiled target architecture
if(UNIX)
  if(APPLE)
    # Default OS X TTY for Arduino Uno
    set(FERMware_PORT /dev/tty.usbmodem1451)
  else()
    # Default Linux TTY for Arduino Uno
    set(FERMware_PORT /dev/ttyACM0)
  endif()
endif()

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)
project(FERMware)


set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/ArduinoToolchain.cmake)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries)

add_subdirectory(src)
