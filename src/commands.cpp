#include "commands.h"
#include "globals.h"
#include "sensors.h"

String current_command = "";


void set_pinstate(const String &cmd)
{
  char buff[10];
  int index = cmd.indexOf(' ');
  String command = cmd.substring(0, index);
  String args = cmd.substring(index+1);

  index = args.indexOf(' ');
  String pin = args.substring(0, index);
  String highlow = args.substring(index+1);
  int pinstate = highlow == "HIGH" ? HIGH : LOW;

  Serial.print("{\"result\": \"Pin ");
  Serial.print(pin + " is ");
  Serial.print(pinstate == LOW ? "ON" : "OFF");
  Serial.println("\"}");

  pin.toCharArray(buff, 10);
  digitalWrite(atoi(buff), pinstate);
}

void system_state()
{
  Serial.print("{\"status\":{\"glycol\":");
  Serial.print(glycol_state == COOLING ? "\"COOLING\"" : "\"IDLE\"");
  Serial.print(",\"bottom\":");
  Serial.print(bottom_zone_state == COOLING ? "\"COOLING\"" : "\"IDLE\"");
  Serial.println("}}");
}

void process_command(const String &command)
{
  if (command == "list_sensors"){
    list_sensors(SENSOR_PIN);
  }
  else if (command == "discover_sensors"){
    discover_sensors(SENSOR_PIN);
  }
  else if (command == "status"){
    system_state();
  }
  else if (command.startsWith("set_pin")){
    set_pinstate(command);
  }
  else {
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
