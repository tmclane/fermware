#include "constants.h"
#include "globals.h"
#include "sensors.h"
#include "system.h"

unsigned long system_last_time;
unsigned long sensor_last_time;

#define ACTIVATION_TIME 10
#define SENSOR_UPDATE_TIME ACTIVATION_TIME / 2

int locate_sensor(const byte address[8])
{
  for (int i=0; i<sensor_count; i++) {
    int matching = 0;
    for (int j=0; j<8; j++) {
      if (sensors[i].address[j] == address[j]){
        matching++;
      }
    }
    if (matching == 8)
      return i;
  }
  return -1;
}

float zone_temperature(const byte address[8])
{
  int index = locate_sensor(address);
  if (index >= 0)
    return sensors[index].fahrenheit;
  else
    return -199.0;
}


// Maintains the system
// Rules are hardcoded for now.
void maintain_system(unsigned long current_time)
{
  if ((current_time - sensor_last_time) / 1000 > SENSOR_UPDATE_TIME || current_time == -1){
    sensor_last_time = current_time;
    Serial.println("Updating sensor values");
    update_sensors(SENSOR_PIN);
  }

  if ((current_time - system_last_time) / 1000 > ACTIVATION_TIME || current_time == -1){
    system_last_time = current_time;

    // Maintain Ale Zone (Bottom Chamber)
    float zone_temp = zone_temperature(BOTTOMCHAMBER_ADDR);
    Serial.print("Bottom Chamber Temp: ");
    Serial.println(zone_temp);
    if (zone_temp != -199.0) {
      if (zone_temp > (bottom_temp_setting + bottom_temp_overshoot) &&
          bottom_zone_state == IDLE){
        digitalWrite(BOTTOMCHAMBER, LOW);  // Enable cooling
        bottom_zone_state = COOLING;
      }

      if (zone_temp <= (bottom_temp_setting - bottom_temp_undershoot) &&
          bottom_zone_state == COOLING){
        digitalWrite(BOTTOMCHAMBER, HIGH);  // Disable cooling
        bottom_zone_state = IDLE;
      }
    }

    // Maintain Lager Zone (Top Chamber)
    // TODO

    // Maintain Cooling System
    zone_temp = zone_temperature(GLYCOL_ADDR);
    Serial.print("Glycol Temp: ");
    Serial.println(zone_temp);
    if ( zone_temp > (glycol_temp_setting + glycol_temp_overshoot) &&
         glycol_state == IDLE){
      Serial.print("Enabling cooling for glycol: Temperature: ");
      Serial.println(zone_temp);
      digitalWrite(AIRCON, LOW);  // Enable cooling
      glycol_state = COOLING;
    }

    if ( zone_temp < (glycol_temp_setting - glycol_temp_undershoot) &&
         glycol_state == COOLING) {
      Serial.print("Disabling cooling for glycol: Temperature: ");
      Serial.println(zone_temp);
      digitalWrite(AIRCON, HIGH);  // Disable cooling
      glycol_state = IDLE;
    }
  }
}
