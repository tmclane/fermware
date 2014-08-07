#ifndef _H_SENSORS
#define _H_SENSORS

struct Sensor {
        char address[8];
        char id;
};

extern struct Sensor* sensors;

void discover_sensors(int);
void discover_sensors_real(int);


#endif
