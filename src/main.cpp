#include <Arduino.h>

#include "commands.h"
#include "constants.h"
#include "globals.h"
#include "sensors.h"
#include "system.h"

unsigned long last_time = -1;

// Arduino Entry Points

void setup()
{
  Serial.begin(SERIAL_SPEED);
  while (!Serial){};

  pinMode(HEARTBEAT_LED, OUTPUT);

  // Initial control pins to HIGH (OFF)
  for (int i=4; i<8; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}

void loop()
{
  unsigned long current_time = millis();
  if (last_time == -1)
    current_time = -1;  // to ensure the code runs right away

  if(current_time - last_time > 1000 || last_time == -1) {
    last_time = current_time;
    ledState = ledState == HIGH ? LOW : HIGH;
    digitalWrite(HEARTBEAT_LED, ledState);

    maintain_system(current_time);

    if (last_time == -1)
      last_time = millis();
  }

  process_commands();

}
