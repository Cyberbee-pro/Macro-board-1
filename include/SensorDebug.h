#ifndef SENSOR_DEBUG_H
#define SENSOR_DEBUG_H

#include <Arduino.h>

constexpr bool SENSOR_DEBUG_MODE = false;
// constexpr bool SENSOR_DEBUG_MODE = true;


// Raw mode is useful when checking ADC wiring and noise.
// Processed mode mirrors the filtered value returned by ResponsiveAnalogRead::getValue().
// Combined mode logs both so calibration can be compared against control behavior.
enum class SensorDebugView : uint8_t {
    Raw,
    Processed,
    Combined,
};

constexpr SensorDebugView SENSOR_DEBUG_VIEW = SensorDebugView::Combined;

void sensor_debug_setup();
void sensor_debug_log();

void sensor_debug_setup_raw();
void sensor_debug_log_raw();

#endif
