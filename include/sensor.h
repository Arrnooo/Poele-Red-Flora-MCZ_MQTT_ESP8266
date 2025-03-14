#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

void setup_sensor();
void handle_sensor();

extern uint16_t distance;

#endif