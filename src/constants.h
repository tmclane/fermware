#ifndef _H_CONSTANTS
#define _H_CONSTANTS

#include <Arduino.h>

#define HEARTBEAT_LED LED_BUILTIN
#define SERIAL_SPEED 115200

#define AIRCON 5
#define BOTTOMCHAMBER 6
#define TOPCHAMBER 7

#define SENSOR_PIN A4

// Temporary defines
const byte GLYCOL_ADDR[8] = {0x28, 0xCE, 0xB9, 0x50, 0x05, 0x00, 0x00, 0xC3};
const byte BOTTOMCHAMBER_ADDR[8] = {0x28, 0xC6, 0x1E, 0x51, 0x05, 0x00, 0x00, 0x8F};

#define IDLE 0
#define COOLING 1

#endif
