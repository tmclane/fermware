#include "globals.h"
#include <Arduino.h>

int ledState = LOW;
unsigned long uptime = 0;

// Initial configuration
float bottom_temp_setting = 62;
float bottom_temp_undershoot = 0.5;
float bottom_temp_overshoot = 0.5;
char bottom_zone_state = 0;

float glycol_temp_setting = 34;
float glycol_temp_overshoot = 10;
float glycol_temp_undershoot = 10;
char glycol_state = 0;
