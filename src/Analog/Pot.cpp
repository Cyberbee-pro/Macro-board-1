#include"../Headers/POTpins.h"
#include"../Headers/PINS.h"

#include <Arduino.h>



void FN_led_pot(){
    POT_State = analogRead(POT_SIG);
    Serial.println(POT_State);
}