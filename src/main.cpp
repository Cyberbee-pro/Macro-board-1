#include <Arduino.h>

#include "Headers/PINS.h"
#include "Headers/POTpins.h"



void setup() {
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  
  // LEP PINS SETUP
  pinMode(LED_UNO,OUTPUT);
  pinMode(LED_DOS,OUTPUT);
  pinMode(LED_TRESS,OUTPUT);

  //Joystick setup
  pinMode(JOY_HOR,INPUT_PULLUP);
  pinMode(JOY_VERT,INPUT_PULLUP);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  FN_led_pot();
}

// put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }