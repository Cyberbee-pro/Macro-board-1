#include"../Headers/POTpins.h"
#include"../Headers/PINS.h"

#include <Arduino.h>

void setup(){
    Serial.begin(9600);
    pinMode(LED_UNO,OUTPUT);
}

void FN_led_pot(){
    POT_State = analogRead(POT_SIG);
    Serial.println(POT_State);
}