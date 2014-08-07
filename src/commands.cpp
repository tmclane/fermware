#include "commands.h"

String current_command = "";

void process_command(const String &command)
{
  Serial.print("Received command: ");
  Serial.print(command);
  Serial.println();
}

void process_commands()
{
  while (Serial.available())
  {
    char value = (char)Serial.read();
    Serial.print(value); // Echo value back to sender (DEBUG)

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
