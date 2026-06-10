#include <Arduino.h>
#include <PINS.h>
#include <JOY.h>

int Threashold_Joy = 150; // Increased to handle raw hardware noise cushions

int Curr_Joy_Y = 0;
int Curr_Joy_X = 0;

void (*call_joy)() = nullptr;

void joy_state_update(void (*call_inp)()){
    if (call_inp != nullptr) {
        call_joy = call_inp;
    }
    // 1. Process Vertical Axis (Y-Axis)
    Joy_Ver_Res.update();
    Curr_Joy_Y = Joy_Ver_Res.getValue();
    
    
    // 2. Process Horizontal Axis (X-Axis)
    Joy_Hor_Res.update();
    Curr_Joy_X = Joy_Hor_Res.getValue();
    
    joy_run();

    // delay(50);
}



void joy_run(){
    if(nullptr != call_joy){
        call_joy();
    }else{
        joy_run_def();
    }
}

void joy_run_def(){
    if(Curr_Joy_Y > 2048 + Threashold_Joy){
        Serial.println("UP");
    } else if (Curr_Joy_Y < 2048 - Threashold_Joy) {
        Serial.println("Down");
    } else {
        // Serial.print("Y Resting               |");
    }

    if(Curr_Joy_X > 2048 + Threashold_Joy){
        Serial.println("Left");
    } else if (Curr_Joy_X < 2048 - Threashold_Joy) {
        Serial.println("Right");
    } else {
        // Serial.print("|         X Resting");
    }

    // Serial.println("");


    Click_Left_reg();
    Click_Right_reg();

}

void Click_Left_reg(){
    if(digitalRead(BUTTON_LEFT)==LOW){
        Serial.println("Left Button pressed");
    }
}


void Click_Right_reg(){
    if(digitalRead(BUTTON_RIGHT)==LOW){
        Serial.println("Right Button pressed");
    }
}
