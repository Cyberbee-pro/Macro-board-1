#include <Arduino.h>

#include <PINS.h>
#include <POT.h>
#include <JOY.h>
#include <ModeDisp.h>
#include <SensorDebug.h>

void setup()
{
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  Serial.begin(115200);
  populate_modes();
  pins_setup();
  sensor_debug_setup();
}

void loop()
{
  if (SENSOR_DEBUG_MODE)
  {
    joy_state_update();
    pot_update();
    sensor_debug_log();
    delay(20);
    return;
  }

  if (bleMouse.isConnected())
  {
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
