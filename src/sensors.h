#ifndef _H_SENSORS
#define _H_SENSORS

#include <Arduino.h>

struct Sensor {
        uint8_t address[8];
        char id;
};

extern struct Sensor* sensors;

void list_sensors(int);


#endif
