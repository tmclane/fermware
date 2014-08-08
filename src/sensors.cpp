#include <OneWire.h>
#include "sensors.h"

int sensor_count = 0;
struct Sensor* sensors = NULL;

void sensor_temperature(OneWire &ds, const byte* addr)
{
  byte type_s;
  byte data[12];
  float celsius, fahrenheit;
  byte present = 0;

  // the first ROM byte indicates which chip
  switch (addr[0]) {
  case 0x10:
    type_s = 1;
    break;
  case 0x28:
    type_s = 0;
    break;
  case 0x22:
    type_s = 0;
    break;
  default:
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 0);        // start conversion, with parasite power on at the end

  //  delay(100);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad

  for (byte i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }

  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;

  Serial.print("\"C\":");
  Serial.print(celsius);
  Serial.print(",\"F\":");
  Serial.print(fahrenheit);
}

void sensor_details(OneWire &ds, const byte* addr)
{
  Serial.print("{\"address\":\"");
  for(int i = 0; i < 7; i++) {
    if (addr[i] < 10)
      Serial.print('0');
    Serial.print(addr[i], HEX);
    Serial.print(':');
  }
  if (addr[7] < 10)
    Serial.print('0');
  Serial.print(addr[7], HEX);
  Serial.print("\",");
  sensor_temperature(ds, addr);
  Serial.print('}');
}

void discover_sensors(int onewire_pin)
{
  OneWire ds(onewire_pin);
  byte addr[8];

  sensor_count = 0;
  if (sensors){
    free(sensors);
    sensors = NULL;
  }

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  else {
    Serial.print('[');
    sensor_details(ds, addr);
    sensor_count++;
  }

  while(ds.search(addr)){
    Serial.print(',');
    sensor_details(ds, addr);
    sensor_count++;
  }
  if (sensor_count)
    Serial.print(']');
}
