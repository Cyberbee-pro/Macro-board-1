#include <Arduino.h>

#include <JOY.h>
#include <PINS.h>
#include <POT.h>
#include <SensorDebug.h>

namespace {

custom_wait sensor_log_wait(75);

int center_axis(int value) {
    return value - 2048;
}

}  // namespace

void sensor_debug_setup() {
    if (!SENSOR_DEBUG_MODE) {
        return;
    }

    sensor_log_wait.start();
    Serial.println("# Sensor debug mode enabled");
    Serial.println("# CSV columns:");
    Serial.println("# ms,joy_x_raw,joy_y_raw,joy_x_centered,joy_y_centered,joy_x_step,joy_y_step,pot_raw,pot_pct");
}

void sensor_debug_log() {
    if (!SENSOR_DEBUG_MODE || !sensor_log_wait.is_Time_Enlapsed()) {
        return;
    }

    Serial.printf(
        "%lu,%d,%d,%d,%d,%d,%d,%d,%d\n",
        millis(),
        Joy_Hor_Res.getRawValue(),
        Joy_Ver_Res.getRawValue(),
        center_axis(Curr_Joy_X),
        center_axis(Curr_Joy_Y),
        Move_X,
        Move_Y,
        Pot_Raw_state,
        Curr_Pot_state
    );
}
