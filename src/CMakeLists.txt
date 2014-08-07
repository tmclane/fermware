set(ARDUINO_DEFAULT_BOARD uno) # Default Board ID, when not specified
set(ARDUINO_DEFAULT_PORT /dev/ttyUSB0) # Default Port, when not specified

# OS X specific (maybe machine specific)
set(FERMware_PORT /dev/tty.usbmodem1451)

set(FERMware_SRCS main.cpp commands.cpp globals.cpp sensors.cpp)

#====================================================================#
# Controller firmware
#====================================================================#
generate_arduino_firmware(FERMware
  BOARD uno
  SRCS ${FERMware_SRCS}
)
