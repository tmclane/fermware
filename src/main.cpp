#include <Arduino.h>

#include "commands.h"
#include "constants.h"
#include "globals.h"

unsigned long last_time;

// Arduino Entry Points

void setup()
{
  Serial.begin(SERIAL_SPEED);
  while (!Serial){};

  pinMode(HEARTBEAT_LED, OUTPUT);
}

void loop()
{
  unsigned long current_time = millis();
  if(current_time - last_time > 1000) {
    last_time = current_time;
    ledState = ledState == HIGH ? LOW : HIGH;
    digitalWrite(HEARTBEAT_LED, ledState);
  }

  process_commands();
}