#include "constants.h"
#include "globals.h"
#include "sensors.h"
#include "system.h"

unsigned long system_last_time;

#define ACTIVATION_TIME 60

float zone_temperature(const byte address[8])
{
  for (int i=0; i<sensor_count; i++) {
    bool found = true;
    for (int j=0; j<8; j++)
      if (sensors[i].address[j] != address[j]){
        found = false;
        break;
      }

    if (found)
      return sensors[i].fahrenheit;
  }

  return -199.0;
}


// Maintains the system
// Rules are hardcoded for now.
void maintain_system(unsigned long current_time)
{
  if ((current_time - system_last_time) / 1000 > ACTIVATION_TIME || current_time == -1){
    // Update our cached temperatures
    update_sensors(SENSOR_PIN);
    delay(100);
    update_sensors(SENSOR_PIN);

    // Maintain Ale Zone (Bottom Chamber)
    float zone_temp = zone_temperature((const byte*)BOTTOMCHAMBER_ADDR);
    if (zone_temp > (bottom_zone_setting - bottom_zone_overshoot))
      digitalWrite(BOTTOMCHAMBER, LOW);  // Enable cooling
    else
      digitalWrite(BOTTOMCHAMBER, HIGH);  // Disable cooling

    // Maintain Lager Zone (Top Chamber)

    // Maintain Cooling System
    zone_temp = zone_temperature(GLYCOL_ADDR);
    if (zone_temp > (glycol_temp_setting - glycol_temp_overshoot))
      digitalWrite(AIRCON, LOW);  // Enable cooling
    else
      digitalWrite(AIRCON, HIGH);  // Disable cooling

    system_last_time = current_time;
  }
}
