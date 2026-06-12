#ifndef ESP32_BLE_MOUSE_H
#define ESP32_BLE_MOUSE_H

#include <Arduino.h>
#include <string>

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4
#define MOUSE_BACK 8
#define MOUSE_FORWARD 16
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)

class BleMouse {
  public:
    BleMouse(
        std::string deviceName = "ESP32 Bluetooth Mouse",
        std::string deviceManufacturer = "Espressif",
        uint8_t batteryLevel = 100
    );

    void begin();
    void end();

    void click(uint8_t button = MOUSE_LEFT);
    void move(signed char x, signed char y, signed char wheel = 0, signed char hWheel = 0);
    void press(uint8_t button = MOUSE_LEFT);
    void release(uint8_t button = MOUSE_LEFT);
    bool isPressed(uint8_t button = MOUSE_LEFT);
    bool isConnected();
    void setBatteryLevel(uint8_t level);

  private:
    std::string deviceName;
    std::string deviceManufacturer;
    uint8_t batteryLevel;
};

#endif
