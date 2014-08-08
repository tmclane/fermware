set(ARDUINO_DEFAULT_BOARD uno) # Default Board ID, when not specified
set(ARDUINO_DEFAULT_PORT /dev/ttyUSB0) # Default Port, when not specified

if(UNIX)
  if(APPLE)
    set(FERMware_PORT /dev/tty.usbmodem1451)
  else()
    set(FERMware_PORT /dev/ttyACM0)
  endif()
endif()

set(FERMware_SRCS main.cpp commands.cpp globals.cpp sensors.cpp)

#====================================================================#
# Controller firmware
#====================================================================#
generate_arduino_firmware(FERMware
  BOARD uno
  SRCS ${FERMware_SRCS}
)
