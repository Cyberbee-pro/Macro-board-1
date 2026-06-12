#include <BleKeyboard.h>
#include <BleMouse.h>

#include <cstring>
#include <utility>

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <BLESecurity.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <HIDTypes.h>
#include <esp_timer.h>

namespace {

constexpr uint8_t KEYBOARD_ID = 0x01;
constexpr uint8_t MEDIA_KEYS_ID = 0x02;
constexpr uint8_t MOUSE_ID = 0x03;
constexpr uint16_t CCCD_UUID = 0x2902;

const MediaKeyReport KEY_MEDIA_EMPTY = {0, 0};

const uint8_t asciiMap[128] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0x2b, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c,
    0x1e | 0x80, 0x34 | 0x80, 0x20 | 0x80, 0x21 | 0x80, 0x22 | 0x80, 0x24 | 0x80, 0x34, 0x26 | 0x80,
    0x27 | 0x80, 0x25 | 0x80, 0x2e | 0x80, 0x36, 0x2d, 0x37, 0x38, 0x27,
    0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x33 | 0x80, 0x33, 0x36 | 0x80, 0x2e, 0x37 | 0x80,
    0x38 | 0x80, 0x1f | 0x80, 0x04 | 0x80, 0x05 | 0x80, 0x06 | 0x80, 0x07 | 0x80, 0x08 | 0x80, 0x09 | 0x80,
    0x0a | 0x80, 0x0b | 0x80, 0x0c | 0x80, 0x0d | 0x80, 0x0e | 0x80, 0x0f | 0x80, 0x10 | 0x80, 0x11 | 0x80,
    0x12 | 0x80, 0x13 | 0x80, 0x14 | 0x80, 0x15 | 0x80, 0x16 | 0x80, 0x17 | 0x80, 0x18 | 0x80, 0x19 | 0x80,
    0x1a | 0x80, 0x1b | 0x80, 0x1c | 0x80, 0x1d | 0x80, 0x2f, 0x31, 0x30, 0x23 | 0x80, 0x2d | 0x80, 0x35,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x2f | 0x80, 0x31 | 0x80, 0x30 | 0x80,
    0x35 | 0x80, 0x00
};

typedef struct {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
} KeyReport;

const uint8_t hidReportDescriptor[] = {
    USAGE_PAGE(1), 0x01,
    USAGE(1), 0x06,
    COLLECTION(1), 0x01,
    REPORT_ID(1), KEYBOARD_ID,
    USAGE_PAGE(1), 0x07,
    USAGE_MINIMUM(1), 0xE0,
    USAGE_MAXIMUM(1), 0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1), 0x01,
    REPORT_COUNT(1), 0x08,
    HIDINPUT(1), 0x02,
    REPORT_COUNT(1), 0x01,
    REPORT_SIZE(1), 0x08,
    HIDINPUT(1), 0x01,
    REPORT_COUNT(1), 0x05,
    REPORT_SIZE(1), 0x01,
    USAGE_PAGE(1), 0x08,
    USAGE_MINIMUM(1), 0x01,
    USAGE_MAXIMUM(1), 0x05,
    HIDOUTPUT(1), 0x02,
    REPORT_COUNT(1), 0x01,
    REPORT_SIZE(1), 0x03,
    HIDOUTPUT(1), 0x01,
    REPORT_COUNT(1), 0x06,
    REPORT_SIZE(1), 0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,
    USAGE_PAGE(1), 0x07,
    USAGE_MINIMUM(1), 0x00,
    USAGE_MAXIMUM(1), 0x65,
    HIDINPUT(1), 0x00,
    END_COLLECTION(0),

    USAGE_PAGE(1), 0x0C,
    USAGE(1), 0x01,
    COLLECTION(1), 0x01,
    REPORT_ID(1), MEDIA_KEYS_ID,
    USAGE_PAGE(1), 0x0C,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1), 0x01,
    REPORT_COUNT(1), 0x10,
    USAGE(1), 0xB5,
    USAGE(1), 0xB6,
    USAGE(1), 0xB7,
    USAGE(1), 0xCD,
    USAGE(1), 0xE2,
    USAGE(1), 0xE9,
    USAGE(1), 0xEA,
    USAGE(2), 0x23, 0x02,
    USAGE(2), 0x94, 0x01,
    USAGE(2), 0x92, 0x01,
    USAGE(2), 0x2A, 0x02,
    USAGE(2), 0x21, 0x02,
    USAGE(2), 0x26, 0x02,
    USAGE(2), 0x24, 0x02,
    USAGE(2), 0x83, 0x01,
    USAGE(2), 0x8A, 0x01,
    HIDINPUT(1), 0x02,
    END_COLLECTION(0),

    USAGE_PAGE(1), 0x01,
    USAGE(1), 0x02,
    COLLECTION(1), 0x01,
    REPORT_ID(1), MOUSE_ID,
    USAGE(1), 0x01,
    COLLECTION(1), 0x00,
    USAGE_PAGE(1), 0x09,
    USAGE_MINIMUM(1), 0x01,
    USAGE_MAXIMUM(1), 0x05,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1), 0x01,
    REPORT_COUNT(1), 0x05,
    HIDINPUT(1), 0x02,
    REPORT_SIZE(1), 0x03,
    REPORT_COUNT(1), 0x01,
    HIDINPUT(1), 0x03,
    USAGE_PAGE(1), 0x01,
    USAGE(1), 0x30,
    USAGE(1), 0x31,
    USAGE(1), 0x38,
    LOGICAL_MINIMUM(1), 0x81,
    LOGICAL_MAXIMUM(1), 0x7F,
    REPORT_SIZE(1), 0x08,
    REPORT_COUNT(1), 0x03,
    HIDINPUT(1), 0x06,
    USAGE_PAGE(1), 0x0C,
    USAGE(2), 0x38, 0x02,
    LOGICAL_MINIMUM(1), 0x81,
    LOGICAL_MAXIMUM(1), 0x7F,
    REPORT_SIZE(1), 0x08,
    REPORT_COUNT(1), 0x01,
    HIDINPUT(1), 0x06,
    END_COLLECTION(0),
    END_COLLECTION(0)
};

void delayMilliseconds(uint32_t ms) {
    if (ms == 0) {
        return;
    }

    const uint64_t start = esp_timer_get_time();
    const uint64_t durationUs = static_cast<uint64_t>(ms) * 1000ULL;
    while ((esp_timer_get_time() - start) < durationUs) {
    }
}

BLE2902 *notificationDescriptor(BLECharacteristic *characteristic) {
    if (characteristic == nullptr) {
        return nullptr;
    }

    return static_cast<BLE2902 *>(
        characteristic->getDescriptorByUUID(BLEUUID(CCCD_UUID))
    );
}

class BleComboCallbacks : public BLEServerCallbacks, public BLECharacteristicCallbacks {
  public:
    bool connected = false;
    BLEAdvertising *advertising = nullptr;
    BLECharacteristic *inputKeyboard = nullptr;
    BLECharacteristic *inputMediaKeys = nullptr;
    BLECharacteristic *inputMouse = nullptr;

    void onConnect(BLEServer *server) override {
        (void)server;
        connected = true;

        if (BLE2902 *descriptor = notificationDescriptor(inputKeyboard)) {
            descriptor->setNotifications(true);
        }
        if (BLE2902 *descriptor = notificationDescriptor(inputMediaKeys)) {
            descriptor->setNotifications(true);
        }
        if (BLE2902 *descriptor = notificationDescriptor(inputMouse)) {
            descriptor->setNotifications(true);
        }
    }

    void onDisconnect(BLEServer *server) override {
        (void)server;
        connected = false;

        if (BLE2902 *descriptor = notificationDescriptor(inputKeyboard)) {
            descriptor->setNotifications(false);
        }
        if (BLE2902 *descriptor = notificationDescriptor(inputMediaKeys)) {
            descriptor->setNotifications(false);
        }
        if (BLE2902 *descriptor = notificationDescriptor(inputMouse)) {
            descriptor->setNotifications(false);
        }

        if (advertising != nullptr) {
            advertising->start();
        }
    }

    void onWrite(BLECharacteristic *characteristic) override {
        (void)characteristic;
    }
};

class BleComboDevice {
  public:
    void begin(const std::string &name, const std::string &manufacturer, uint8_t batteryLevel) {
        if (started) {
            return;
        }

        deviceName = name.empty() ? "MacroBoard V1" : name;
        deviceManufacturer = manufacturer.empty() ? "Saa-labs" : manufacturer;
        battery = batteryLevel;

        BLEDevice::init(deviceName);
        BLEServer *server = BLEDevice::createServer();
        server->setCallbacks(&callbacks);

        hid = new BLEHIDDevice(server);
        inputKeyboard = hid->inputReport(KEYBOARD_ID);
        outputKeyboard = hid->outputReport(KEYBOARD_ID);
        inputMediaKeys = hid->inputReport(MEDIA_KEYS_ID);
        inputMouse = hid->inputReport(MOUSE_ID);

        outputKeyboard->setCallbacks(&callbacks);
        callbacks.inputKeyboard = inputKeyboard;
        callbacks.inputMediaKeys = inputMediaKeys;
        callbacks.inputMouse = inputMouse;

        hid->manufacturer()->setValue(deviceManufacturer);
        hid->pnp(0x02, vid, pid, version);
        hid->hidInfo(0x00, 0x01);

        BLESecurity *security = new BLESecurity();
        security->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);

        hid->reportMap(const_cast<uint8_t *>(hidReportDescriptor), sizeof(hidReportDescriptor));
        hid->startServices();

        advertising = server->getAdvertising();
        callbacks.advertising = advertising;
        advertising->setAppearance(HID_KEYBOARD);
        advertising->addServiceUUID(hid->hidService()->getUUID());
        advertising->setScanResponse(false);
        advertising->start();

        hid->setBatteryLevel(battery);
        started = true;
    }

    bool isConnected() const {
        return callbacks.connected;
    }

    void setBatteryLevel(uint8_t level) {
        battery = level;
        if (hid != nullptr) {
            hid->setBatteryLevel(level);
        }
    }

    void setName(const std::string &name) {
        if (!started) {
            deviceName = name;
        }
    }

    void setDelay(uint32_t delayMs) {
        keyDelayMs = delayMs;
    }

    void setVendorId(uint16_t vendorId) {
        vid = vendorId;
    }

    void setProductId(uint16_t productId) {
        pid = productId;
    }

    void setVersion(uint16_t deviceVersion) {
        version = deviceVersion;
    }

    void mouseMove(signed char x, signed char y, signed char wheel, signed char hWheel) {
        if (!isConnected() || inputMouse == nullptr) {
            return;
        }

        uint8_t report[5] = {
            mouseButtons,
            static_cast<uint8_t>(x),
            static_cast<uint8_t>(y),
            static_cast<uint8_t>(wheel),
            static_cast<uint8_t>(hWheel)
        };

        inputMouse->setValue(report, sizeof(report));
        inputMouse->notify();
    }

    void mouseClick(uint8_t button) {
        mouseButtons = button;
        mouseMove(0, 0, 0, 0);
        mouseButtons = 0;
        mouseMove(0, 0, 0, 0);
    }

    void mousePress(uint8_t button) {
        if ((mouseButtons & button) == button) {
            return;
        }

        mouseButtons |= button;
        mouseMove(0, 0, 0, 0);
    }

    void mouseRelease(uint8_t button) {
        if ((mouseButtons & button) == 0) {
            return;
        }

        mouseButtons &= static_cast<uint8_t>(~button);
        mouseMove(0, 0, 0, 0);
    }

    bool mouseIsPressed(uint8_t button) const {
        return (mouseButtons & button) != 0;
    }

    size_t keyboardPress(uint8_t key) {
        uint8_t keyCode = key;

        if (keyCode >= 136) {
            keyCode = keyCode - 136;
        } else if (keyCode >= 128) {
            keyReport.modifiers |= static_cast<uint8_t>(1U << (keyCode - 128));
            keyCode = 0;
        } else {
            keyCode = pgm_read_byte(asciiMap + keyCode);
            if (keyCode == 0) {
                return 0;
            }

            if ((keyCode & 0x80U) != 0U) {
                keyReport.modifiers |= 0x02;
                keyCode &= 0x7FU;
            }
        }

        if (keyCode != 0) {
            for (uint8_t existingKey : keyReport.keys) {
                if (existingKey == keyCode) {
                    sendKeyboardReport();
                    return 1;
                }
            }

            for (uint8_t &slot : keyReport.keys) {
                if (slot == 0x00) {
                    slot = keyCode;
                    sendKeyboardReport();
                    return 1;
                }
            }

            return 0;
        }

        sendKeyboardReport();
        return 1;
    }

    size_t keyboardPress(const MediaKeyReport key) {
        const uint16_t keyMask = key[1] | (static_cast<uint16_t>(key[0]) << 8);
        uint16_t currentMask = mediaKeyReport[1] | (static_cast<uint16_t>(mediaKeyReport[0]) << 8);
        currentMask |= keyMask;
        mediaKeyReport[0] = static_cast<uint8_t>((currentMask & 0xFF00U) >> 8);
        mediaKeyReport[1] = static_cast<uint8_t>(currentMask & 0x00FFU);
        sendMediaReport();
        return 1;
    }

    size_t keyboardRelease(uint8_t key) {
        uint8_t keyCode = key;

        if (keyCode >= 136) {
            keyCode = keyCode - 136;
        } else if (keyCode >= 128) {
            keyReport.modifiers &= static_cast<uint8_t>(~(1U << (keyCode - 128)));
            keyCode = 0;
        } else {
            keyCode = pgm_read_byte(asciiMap + keyCode);
            if (keyCode == 0) {
                return 0;
            }

            if ((keyCode & 0x80U) != 0U) {
                keyReport.modifiers &= static_cast<uint8_t>(~0x02U);
                keyCode &= 0x7FU;
            }
        }

        if (keyCode != 0) {
            for (uint8_t &slot : keyReport.keys) {
                if (slot == keyCode) {
                    slot = 0x00;
                }
            }
        }

        sendKeyboardReport();
        return 1;
    }

    size_t keyboardRelease(const MediaKeyReport key) {
        const uint16_t keyMask = key[1] | (static_cast<uint16_t>(key[0]) << 8);
        uint16_t currentMask = mediaKeyReport[1] | (static_cast<uint16_t>(mediaKeyReport[0]) << 8);
        currentMask &= static_cast<uint16_t>(~keyMask);
        mediaKeyReport[0] = static_cast<uint8_t>((currentMask & 0xFF00U) >> 8);
        mediaKeyReport[1] = static_cast<uint8_t>(currentMask & 0x00FFU);
        sendMediaReport();
        return 1;
    }

    void keyboardReleaseAll() {
        std::memset(&keyReport, 0, sizeof(keyReport));
        std::memcpy(mediaKeyReport, KEY_MEDIA_EMPTY, sizeof(MediaKeyReport));
        sendKeyboardReport();
        sendMediaReport();
    }

    size_t keyboardWrite(uint8_t key) {
        const size_t pressed = keyboardPress(key);
        keyboardRelease(key);
        return pressed;
    }

    size_t keyboardWrite(const MediaKeyReport key) {
        const size_t pressed = keyboardPress(key);
        keyboardRelease(key);
        return pressed;
    }

    size_t keyboardWrite(const uint8_t *buffer, size_t size) {
        size_t written = 0;
        while (size-- > 0) {
            if (*buffer != '\r' && keyboardWrite(*buffer) != 0) {
                written++;
            }
            buffer++;
        }
        return written;
    }

  private:
    void sendKeyboardReport() {
        if (!isConnected() || inputKeyboard == nullptr) {
            return;
        }

        inputKeyboard->setValue(reinterpret_cast<uint8_t *>(&keyReport), sizeof(KeyReport));
        inputKeyboard->notify();
        delayMilliseconds(keyDelayMs);
    }

    void sendMediaReport() {
        if (!isConnected() || inputMediaKeys == nullptr) {
            return;
        }

        inputMediaKeys->setValue(mediaKeyReport, sizeof(MediaKeyReport));
        inputMediaKeys->notify();
        delayMilliseconds(keyDelayMs);
    }

    bool started = false;
    std::string deviceName = "MacroBoard V1";
    std::string deviceManufacturer = "Saa-labs";
    uint8_t battery = 100;
    uint16_t vid = 0x05ac;
    uint16_t pid = 0x820a;
    uint16_t version = 0x0210;
    uint32_t keyDelayMs = 7;
    uint8_t mouseButtons = 0;
    KeyReport keyReport = {};
    MediaKeyReport mediaKeyReport = {0, 0};
    BLEHIDDevice *hid = nullptr;
    BLECharacteristic *inputKeyboard = nullptr;
    BLECharacteristic *outputKeyboard = nullptr;
    BLECharacteristic *inputMediaKeys = nullptr;
    BLECharacteristic *inputMouse = nullptr;
    BLEAdvertising *advertising = nullptr;
    BleComboCallbacks callbacks;
};

BleComboDevice comboDevice;

}  // namespace

const MediaKeyReport KEY_MEDIA_VOLUME_UP = {32, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {64, 0};

BleKeyboard::BleKeyboard(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel)
    : deviceName(std::move(deviceName)),
      deviceManufacturer(std::move(deviceManufacturer)),
      batteryLevel(batteryLevel) {
}

void BleKeyboard::begin() {
    comboDevice.begin(deviceName, deviceManufacturer, batteryLevel);
}

void BleKeyboard::end() {
}

size_t BleKeyboard::press(uint8_t key) {
    return comboDevice.keyboardPress(key);
}

size_t BleKeyboard::press(const MediaKeyReport key) {
    return comboDevice.keyboardPress(key);
}

size_t BleKeyboard::release(uint8_t key) {
    return comboDevice.keyboardRelease(key);
}

size_t BleKeyboard::release(const MediaKeyReport key) {
    return comboDevice.keyboardRelease(key);
}

size_t BleKeyboard::write(uint8_t key) {
    return comboDevice.keyboardWrite(key);
}

size_t BleKeyboard::write(const MediaKeyReport key) {
    return comboDevice.keyboardWrite(key);
}

size_t BleKeyboard::write(const uint8_t *buffer, size_t size) {
    return comboDevice.keyboardWrite(buffer, size);
}

void BleKeyboard::releaseAll() {
    comboDevice.keyboardReleaseAll();
}

bool BleKeyboard::isConnected() {
    return comboDevice.isConnected();
}

void BleKeyboard::setBatteryLevel(uint8_t level) {
    batteryLevel = level;
    comboDevice.setBatteryLevel(level);
}

void BleKeyboard::setName(std::string name) {
    deviceName = std::move(name);
    comboDevice.setName(deviceName);
}

void BleKeyboard::setDelay(uint32_t ms) {
    comboDevice.setDelay(ms);
}

void BleKeyboard::set_vendor_id(uint16_t vid) {
    comboDevice.setVendorId(vid);
}

void BleKeyboard::set_product_id(uint16_t pid) {
    comboDevice.setProductId(pid);
}

void BleKeyboard::set_version(uint16_t version) {
    comboDevice.setVersion(version);
}

BleMouse::BleMouse(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel)
    : deviceName(std::move(deviceName)),
      deviceManufacturer(std::move(deviceManufacturer)),
      batteryLevel(batteryLevel) {
}

void BleMouse::begin() {
    comboDevice.begin(deviceName, deviceManufacturer, batteryLevel);
}

void BleMouse::end() {
}

void BleMouse::click(uint8_t button) {
    comboDevice.mouseClick(button);
}

void BleMouse::move(signed char x, signed char y, signed char wheel, signed char hWheel) {
    comboDevice.mouseMove(x, y, wheel, hWheel);
}

void BleMouse::press(uint8_t button) {
    comboDevice.mousePress(button);
}

void BleMouse::release(uint8_t button) {
    comboDevice.mouseRelease(button);
}

bool BleMouse::isPressed(uint8_t button) {
    return comboDevice.mouseIsPressed(button);
}

bool BleMouse::isConnected() {
    return comboDevice.isConnected();
}

void BleMouse::setBatteryLevel(uint8_t level) {
    batteryLevel = level;
    comboDevice.setBatteryLevel(level);
}
