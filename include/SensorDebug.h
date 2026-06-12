#ifndef SENSOR_DEBUG_H
#define SENSOR_DEBUG_H

#include <Arduino.h>

constexpr bool SENSOR_DEBUG_MODE = true;

void sensor_debug_setup();
void sensor_debug_log();

#endif
