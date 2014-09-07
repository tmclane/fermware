#ifndef _H_ZONE
#define _H_ZONE

#include "constants.h"

#define HYSTERESIS 0
#define PID 1

typedef struct HysteresisZone {
  char type;
  char state;
  int sensor;
  char cooling_activator;
  char heating_activator;
  double setpoint;
  double overshoot;
  double undershoot;
  double _filler;    // to make the structs the same size
} HysteresisZone;

typedef struct PIDZone {
  char type;
  char state;
  int sensor;
  char cooling_activator;
  char heating_activator;
  double setpoint;
  double kp;
  double ki;
  double kd;
} PIDZone;

void zones_init();

#endif
