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

int pot_percent(int value) {
    return static_cast<int>(map(value, 0, 4095, 0, 100));
}

void print_sensor_debug_header(SensorDebugView view) {
    Serial.println("# Sensor debug mode enabled");
    Serial.println("# CSV columns:");

    // Keep the column layout aligned with the selected view so the log stays easy to parse.
    if (view == SensorDebugView::Raw) {
        Serial.println("# ms,joy_x_raw,joy_y_raw,joy_x_centered,joy_y_centered,joy_x_step,joy_y_step,pot_raw,pot_pct");
    } else if (view == SensorDebugView::Processed) {
        Serial.println("# ms,joy_x_value,joy_y_value,joy_x_step,joy_y_step,pot_value,pot_pct");
    } else {
        Serial.println("# ms,joy_x_raw,joy_y_raw,joy_x_value,joy_y_value,joy_x_step,joy_y_step,pot_raw,pot_value,pot_pct");
    }
}

void sensor_debug_setup_impl(SensorDebugView view) {
    if (!SENSOR_DEBUG_MODE) {
        return;
    }

    sensor_log_wait.start();
    print_sensor_debug_header(view);
}

void sensor_debug_log_impl(SensorDebugView view) {
    if (!SENSOR_DEBUG_MODE || !sensor_log_wait.is_Time_Enlapsed()) {
        return;
    }

    const int joy_x_raw = Joy_Hor_Res.getRawValue();
    const int joy_y_raw = Joy_Ver_Res.getRawValue();
    const int joy_x_value = Joy_Hor_Res.getValue();
    const int joy_y_value = Joy_Ver_Res.getValue();
    const int pot_raw = Pot_Raw_state;
    const int pot_value = Pot_Sig_Res.getValue();
    const int pot_pct = pot_percent(pot_value);

    // Raw mode keeps the ADC values visible for calibration and wiring checks.
    if (view == SensorDebugView::Raw) {
        Serial.printf(
            "%lu,%d,%d,%d,%d,%d,%d,%d,%d\n",
            millis(),
            joy_x_raw,
            joy_y_raw,
            center_axis(joy_x_raw),
            center_axis(joy_y_raw),
            Move_X,
            Move_Y,
            pot_raw,
            pot_pct
        );
        return;
    }

    // Processed mode matches the values that the control code actually uses.
    if (view == SensorDebugView::Processed) {
        Serial.printf(
            "%lu,%d,%d,%d,%d,%d,%d\n",
            millis(),
            joy_x_value,
            joy_y_value,
            Move_X,
            Move_Y,
            pot_value,
            pot_pct
        );
        return;
    }

    // Combined mode compares raw ADC input with the filtered values driving controls.
    Serial.printf(
        "%lu,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
        millis(),
        joy_x_raw,
        joy_y_raw,
        joy_x_value,
        joy_y_value,
        Move_X,
        Move_Y,
        pot_raw,
        pot_value,
        pot_pct
    );
}

}  // namespace

void sensor_debug_setup_raw() {
    sensor_debug_setup_impl(SensorDebugView::Raw);
}

void sensor_debug_log_raw() {
    sensor_debug_log_impl(SensorDebugView::Raw);
}

void sensor_debug_setup() {
    sensor_debug_setup_impl(SENSOR_DEBUG_VIEW);
}


void sensor_debug_log() {
    sensor_debug_log_impl(SENSOR_DEBUG_VIEW);
}
