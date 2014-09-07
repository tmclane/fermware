#ifndef _H_SENSORS
#define _H_SENSORS

#include <Arduino.h>

struct Sensor
{
  uint8_t address[8];
  float celsius;
  float fahrenheit;
  float celsius_avg;
  float fahrenheit_avg;
  float c_samples[3];
  float f_samples[3];
  int sample;
};

extern struct Sensor* sensors;
extern int sensor_count;

void discover_sensors(int);
void list_sensors(int);
void update_sensors(int);

#endif
