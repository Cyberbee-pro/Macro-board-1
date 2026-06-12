#include <Arduino.h>

#include <PINS.h>
#include <POT.h>
#include <JOY.h>
#include <ModeDisp.h>

void setup()
{
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  Serial.begin(115200);
  populate_modes();
  pins_setup();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // FN_led_pot();
  // if (digitalRead(KEY_TOP_LEFT) == LOW) {
  //   // Fire your target macro trigger event profile here
  //   Serial.println("Top Left Button Action Activated!");
  // }

  if (bleMouse.isConnected())
  {
    Serial.println("TRY work");

    Mode_switch();
    Mode_run();
    Mode_show();
    delay(5);
  }
  else
  {
    Serial.println("Macroboard is not connected , waiting for connection. . . .");
    delay(5000);
  }
}
