#include <Arduino.h>
#include <VirtualButton.h>
#include <BleKeyboard.h>
#include <PINS.h>


VirtualButton::VirtualButton(uint8_t key) : keycode(key), isPressed(false), wasPressed(false) {}

void VirtualButton::update(bool currentlyActive) {
            isPressed = currentlyActive;

            // Edge Detection: State changed from Unpressed -> Pressed
            if (isPressed && !wasPressed) {
                bleKeyboard.press(keycode);
            }
            // Edge Detection: State changed from Pressed -> Unpressed
            else if (!isPressed && wasPressed) {
                bleKeyboard.release(keycode);
            }

            // Save the current state for the next loop
            wasPressed = isPressed;
        }