#include "constants.h"

unsigned long last_run;

void setup()
{

}

void loop()
{
  unsigned long current_time = millis();
  if(current_time - last_run > 1000) {
    last_run = current_time;
    ledState = ledState == HIGH ? LOW : HIGH;
    digitalWrite(ledPin, ledState);
    // FIXME: Do work here
  }

}
