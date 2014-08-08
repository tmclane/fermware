#include "commands.h"
#include "sensors.h"

String current_command = "";

void process_command(const String &command)
{
  if (command == "list_sensors"){
      discover_sensors(A4);
  }
  else
    {
      Serial.print("{\"status\": \"error\", \"message\": \"Received command: '");
      Serial.print(command);
      Serial.println("'\"}");
    }
}

void process_commands()
{
  while (Serial.available())
  {
    char value = (char)Serial.read();

#ifdef DEBUG
    // Echo value back to sender
    Serial.print(value);
#endif

    if (value == '\r' || value == '\n')
    {
      if (current_command == "")
        continue;

      process_command(current_command);

      current_command = "";
    }
    else
      current_command += value;
  }
}
