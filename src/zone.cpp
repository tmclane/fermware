#include "globals.h"
#include "zone.h"

void zones_init() {
  bottom_temp_setting = 62;
  bottom_temp_undershoot = 0.5;
  bottom_temp_overshoot = 0.5;
  bottom_zone_state = IDLE;

  glycol_temp_setting = 34;
  glycol_temp_overshoot = 10;
  glycol_temp_undershoot = 10;
  glycol_state = IDLE;
}
