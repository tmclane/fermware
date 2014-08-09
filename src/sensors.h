#ifndef _H_SENSORS
#define _H_SENSORS

#include <Arduino.h>

struct Sensor
{
  uint8_t address[8];
  float celsius;
  float fahrenheit;
};

extern struct Sensor* sensors;

void list_sensors(int);
void update_sensors(int);

#endif
