#ifndef ESP32_BLE_KEYBOARD_H
#define ESP32_BLE_KEYBOARD_H

#include <Arduino.h>
#include <Print.h>
#include <string>

constexpr uint8_t KEY_LEFT_SHIFT = 0x81;
constexpr uint8_t KEY_UP_ARROW = 0xDA;
constexpr uint8_t KEY_DOWN_ARROW = 0xD9;
constexpr uint8_t KEY_LEFT_ARROW = 0xD8;
constexpr uint8_t KEY_RIGHT_ARROW = 0xD7;
constexpr uint8_t KEY_TAB = 0xB3;
constexpr uint8_t KEY_RETURN = 0xB0;
constexpr uint8_t KEY_ESC = 0xB1;

typedef uint8_t MediaKeyReport[2];

extern const MediaKeyReport KEY_MEDIA_VOLUME_UP;
extern const MediaKeyReport KEY_MEDIA_VOLUME_DOWN;

class BleKeyboard : public Print {
  public:
    BleKeyboard(
        std::string deviceName = "ESP32 Keyboard",
        std::string deviceManufacturer = "Espressif",
        uint8_t batteryLevel = 100
    );

    void begin();
    void end();

    size_t press(uint8_t key);
    size_t press(const MediaKeyReport key);

    size_t release(uint8_t key);
    size_t release(const MediaKeyReport key);

    size_t write(uint8_t key) override;
    size_t write(const MediaKeyReport key);
    size_t write(const uint8_t *buffer, size_t size) override;

    void releaseAll();

    bool isConnected();
    void setBatteryLevel(uint8_t level);
    void setName(std::string deviceName);
    void setDelay(uint32_t ms);
    void set_vendor_id(uint16_t vid);
    void set_product_id(uint16_t pid);
    void set_version(uint16_t version);

  private:
    std::string deviceName;
    std::string deviceManufacturer;
    uint8_t batteryLevel;
};

#endif
