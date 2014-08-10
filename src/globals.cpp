#include "globals.h"
#include <Arduino.h>

int ledState = LOW;

// Initial configuration
int bottom_temp_setting = 65;
int bottom_temp_undershoot = 1;
int bottom_temp_overshoot = .5;
int bottom_zone_state = 0;

int glycol_temp_setting = 34;
int glycol_temp_overshoot = 10;
int glycol_temp_undershoot = 10;
int glycol_state = 0;
