#include <OneWire.h>
#include "constants.h"
#include "sensors.h"

int sensor_count = 0;
struct Sensor* sensors = NULL;

OneWire onewire(SENSOR_PIN);

void discover_sensors(int);

void sensor_temperature(OneWire &ds, const byte* addr, float &celsius, float &fahrenheit)
{
  byte type_s;
  byte data[12];
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
  ds.write(0x44, 0);

  delay(200);     // maybe 750ms is enough, maybe not
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
}

void sensor_details(OneWire &ds, struct Sensor& sensor)
{
  Serial.print("{\"address\":\"");
  for(int i = 0; i < 7; i++) {
    if (sensor.address[i] < 10)
      Serial.print('0');
    Serial.print(sensor.address[i], HEX);
    Serial.print(':');
  }
  if (sensor.address[7] < 10)
    Serial.print('0');

  Serial.print(sensor.address[7], HEX);
  Serial.print("\",");
  Serial.print("\"C\":");
  Serial.print(sensor.celsius);
  Serial.print(",\"F\":");
  Serial.print(sensor.fahrenheit);
  Serial.print(",\"F_AVG\":");
  Serial.print(sensor.fahrenheit_avg);
  Serial.print(",\"C_AVG\":");
  Serial.print(sensor.celsius_avg);
  Serial.print('}');
}

int count_sensors(OneWire &ds)
{
  byte addr[8];

  int count = 0;

  if ( !ds.search(addr)) {
    ds.reset_search();
    return 0;
  }
  else {
    count++;
  }

  while(ds.search(addr)){
    count++;
  }

  return count;
}

void discover_sensors(int onewire_pin)
{
  int processed = 0;
  int count = count_sensors(onewire);

  if (sensors){
    free(sensors);
    sensor_count = 0;
  }

  if (count){
    int sensor_bytes = sizeof(struct Sensor) * count;
    sensors = (struct Sensor*)malloc(sensor_bytes);
    memset(sensors, 0, sensor_bytes);
  }

  onewire.reset_search();
  while(processed < count && onewire.search(sensors[processed].address)){
    processed++;
  }

  sensor_count = processed;
}

void list_sensors(int onewire_pin)
{
  if (!sensors)
    discover_sensors(onewire_pin);

  if (!sensor_count){
    Serial.println("[]\n");  // No sensors present
    return;
  }
  else {
    Serial.print('[');
    sensor_details(onewire, sensors[0]);
  }
  for (int i=1; i<sensor_count; i++){
    Serial.print(',');
    sensor_details(onewire, sensors[i]);
  }
  if (sensor_count)
    Serial.print("]\n");
}

float average(float values[], int count)
{
  float total = 0.0;
  for (int i=0; i<count; i++) {
    total += values[i];
  }

  return total / (float)count;
}

void update_sensors(int onewire_pin)
{
  int processed = 0;
  float current_f;
  float current_c;

  while (!sensor_count){
#ifdef DEBUG
    Serial.println("{\"message\": \"FATAL: No sensors present!\"}");
#endif
    discover_sensors(onewire_pin);
  }

  for (int i=0; i<sensor_count; i++){
    int sample = sensors[i].sample;

    sensor_temperature(onewire, sensors[i].address,
                       current_c, current_f);

    // Check for bogus readings
    if (current_f < -60 && current_f > 100)
      continue;

    sensors[i].celsius = current_c;
    sensors[i].fahrenheit = current_f;

    sensors[i].f_samples[sample] = current_f;
    sensors[i].c_samples[sample] = current_c;

    if (sample == 2) {
      sensors[i].fahrenheit_avg = average(sensors[i].f_samples, 3);
      sensors[i].celsius_avg = average(sensors[i].c_samples, 3);

      sensors[i].sample = 0;
      sample = 0;
    }
    else
      sensors[i].sample++;
  }
}
