#ifndef VB_H
#define VB_H

#include <Arduino.h>



class VirtualButton {
    private:
        uint8_t keycode;
        bool isPressed;
        bool wasPressed;

    public:
        VirtualButton(uint8_t key);
        void update(bool currentlyActive);
    };

#endif