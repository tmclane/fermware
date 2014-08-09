#include "constants.h"
#include "sensors.h"
#include "system.h"

unsigned long system_last_time;

#define ACTIVATION_TIME 60


// Maintains the system
// Rules are hardcoded for now.
void maintain_system(unsigned long current_time)
{
  if ((current_time - system_last_time) / 1000 > ACTIVATION_TIME ){
    update_sensors(SENSOR_PIN);

    // Maintain Ale Zone (Bottom Chamber)


    // Maintain Lager Zone (Top Chamber)


    // Maintain Cooling System


    system_last_time = current_time;
  }
}
