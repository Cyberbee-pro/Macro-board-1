#include <Arduino.h>

#include <PINS.h>
#include <POT.h>



void setup() {
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  Serial.begin(115200);
  
  pins_setup();

}

void loop() {
  // put your main code here, to run repeatedly:
  // FN_led_pot();
if (digitalRead(KEY_TOP_LEFT) == LOW) {
    // Fire your target macro trigger event profile here
    Serial.println("Top Left Button Action Activated!");
  }

}
