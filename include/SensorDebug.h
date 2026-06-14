#ifndef SENSOR_DEBUG_H
#define SENSOR_DEBUG_H

#include <Arduino.h>

constexpr bool SENSOR_DEBUG_MODE = true;

// Raw mode is useful when checking ADC wiring and noise.
// Processed mode mirrors the filtered value returned by ResponsiveAnalogRead::getValue().
enum class SensorDebugView : uint8_t {
    Raw,
    Processed,
};

constexpr SensorDebugView SENSOR_DEBUG_VIEW = SensorDebugView::Processed;

void sensor_debug_setup();
void sensor_debug_log();

void sensor_debug_setup_raw();
void sensor_debug_log_raw();

#endif
