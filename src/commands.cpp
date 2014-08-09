#include "commands.h"
#include "sensors.h"

String current_command = "";


void set_pinstate(const String &cmd)
{
  int index = cmd.indexOf(' ');
  String command = cmd.substring(0, index);
  String args = cmd.substring(index+1);
  index = args.indexOf(' ');
  String pin = args.substring(0, index);
  String highlow = args.substring(index+1);
  int pinstate = highlow == "HIGH" ? HIGH : LOW;
  Serial.print("Setting pin: ");
  Serial.print(pin + "=");
  Serial.println(pinstate == HIGH ? "ON" : "OFF");

  int pinNum = 0;
  char buff[10];

  pin.toCharArray(buff, 10);
  pinNum = atoi(buff);
  digitalWrite(pinNum, pinstate);
}

void process_command(const String &command)
{
  if (command == "list_sensors"){
    discover_sensors(A4);
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
