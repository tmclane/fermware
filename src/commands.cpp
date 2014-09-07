#include "commands.h"
#include "globals.h"
#include "sensors.h"

char cmd_buff[100];
char cmd_location = 0;

void clear_command_buffer() {
  memset(cmd_buff, '\0', 100);
  cmd_location = 0;
}

void set_global(const String &cmd)
{
  char buff[10];
  int index = cmd.indexOf(' ');
  String command = cmd.substring(0, index);
  String args = cmd.substring(index+1);

  index = args.indexOf(' ');
  String variable = args.substring(0, index);
  args.substring(index+1).toCharArray(buff, 10);

  Serial.print("{\"status\": \"success\",\"message\":");

  if (command == "bottom_temp"){
    bottom_temp_setting = atoi(buff);
    Serial.print("\"Set 'bottom_temp' to ");
    Serial.print(bottom_temp_setting);
    Serial.println("\"}");
  }
  if (command == "bottom_undershoot"){
    bottom_temp_undershoot = atoi(buff);
    Serial.print("\"Set 'bottom_temp_undershoot' to ");
    Serial.print(bottom_temp_undershoot);
    Serial.println("\"}");
  }
  if (command == "bottom_overshoot"){
    bottom_temp_overshoot = atoi(buff);
    Serial.print("\"Set 'bottom_temp_overshoot' to ");
    Serial.print(bottom_temp_overshoot);
    Serial.println("\"}");
  }

  if (command == "glycol_temp"){
    glycol_temp_setting = atoi(buff);
    Serial.print("\"Set 'glycol_temp' to ");
    Serial.print(glycol_temp_setting);
    Serial.println("\"}");
  }
  if (command == "glycol_undershoot"){
    glycol_temp_undershoot = atoi(buff);
    Serial.print("\"Set 'glycol_temp_undershoot' to ");
    Serial.print(glycol_temp_undershoot);
    Serial.println("\"}");
  }
  if (command == "bottom_overshoot"){
    glycol_temp_overshoot = atoi(buff);
    Serial.print("\"Set 'glycol_temp_overshoot' to ");
    Serial.print(glycol_temp_overshoot);
    Serial.println("\"}");
  }
}

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
  Serial.print("{\"status\":");
  Serial.print("\"uptime\":");
  Serial.print(uptime);
  Serial.print(",");
  Serial.print("{\"glycol\":{\"state\":");
  Serial.print(glycol_state == COOLING ? "\"COOLING\"" : "\"IDLE\"");
  Serial.print(",\"setpoint\":");
  Serial.print(glycol_temp_setting);
  Serial.print(",\"overshoot\":");
  Serial.print(glycol_temp_overshoot);
  Serial.print(",\"undershoot\":");
  Serial.print(glycol_temp_undershoot);
  Serial.print("},\"bottom\":{\"state\":");
  Serial.print(bottom_zone_state == COOLING ? "\"COOLING\"" : "\"IDLE\"");
  Serial.print(",\"setpoint\":");
  Serial.print(bottom_temp_setting);
  Serial.print(",\"overshoot\":");
  Serial.print(bottom_temp_overshoot);
  Serial.print(",\"undershoot\":");
  Serial.print(bottom_temp_undershoot);
  Serial.println("}}}");
}

void process_command()
{
  String command(cmd_buff);

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
  else if (command.startsWith("set_global")){
    set_global(command);
  }
  else {
    Serial.print("{\"status\": \"error\", \"message\": \"Received command: '");
    Serial.print(command);
    Serial.println("'\"}");
  }
  Serial.flush();
  clear_command_buffer();
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

    if (value == '\n')
    {
      if (cmd_location != 0)
        process_command();
    }
    else {
      cmd_buff[cmd_location++] = value;

      if (cmd_location >= 100)
        clear_command_buffer();
    }
  }
}
