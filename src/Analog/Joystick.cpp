#include <Arduino.h>
#include <PINS.h>
#include <JOY.h>

int Threashold_Joy = 150; // Increased to handle raw hardware noise cushions

int Curr_Joy_Y = 0;
int Curr_Joy_X = 0;

void joy_state_update(){

    // 1. Process Vertical Axis (Y-Axis)
    Joy_Ver_Res.update();
    Curr_Joy_Y = Joy_Ver_Res.getValue();
    
    if(Curr_Joy_Y > 2048 + Threashold_Joy){
        Serial.print("UP                      |");
    } else if (Curr_Joy_Y < 2048 - Threashold_Joy) {
        Serial.print("Down                    |");
    } else {
        Serial.print("Y Resting               |");
    }

    // 2. Process Horizontal Axis (X-Axis)
    Joy_Hor_Res.update();
    Curr_Joy_X = Joy_Hor_Res.getValue();
    
    if(Curr_Joy_X > 2048 + Threashold_Joy){
        Serial.print("|         Left");
    } else if (Curr_Joy_X < 2048 - Threashold_Joy) {
        Serial.print("|         Right");
    } else {
        Serial.print("|         X Resting");
    }

    Serial.println("");
    delay(50);
}