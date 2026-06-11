#include <Arduino.h>
#include <PINS.h>
#include <JOY.h>
#include <BleMouse.h>

int Threashold_Joy = 150; // Increased to handle raw hardware noise cushions
int Sensi = 15; 


int Curr_Joy_Y = 0;
int Curr_Joy_X = 0;

int Move_Y = 0;
int Move_X = 0;

void (*call_joy)() = nullptr;

void joy_state_update(void (*call_inp)()){ 
    if (call_inp != nullptr) {
        call_joy = call_inp;
    }
    // 1. Process Vertical Axis (Y-Axis)
    Joy_Ver_Res.update();
    Curr_Joy_Y = Joy_Ver_Res.getValue();
    Move_X = map(Curr_Joy_X,0,4095,-Sensi,Sensi);
    
    // 2. Process Horizontal Axis (X-Axis)
    Joy_Hor_Res.update();
    Curr_Joy_X = Joy_Hor_Res.getValue();
    Move_Y = map(Curr_Joy_Y,0,4095,-Sensi,Sensi);

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
    // if(Curr_Joy_Y > 2048 + Threashold_Joy){
    //     Serial.println("UP");
    // } else if (Curr_Joy_Y < 2048 - Threashold_Joy) {
    //     Serial.println("Down");
    // } else {
    //     // Serial.print("Y Resting               |");
    // }

    // if(Curr_Joy_X > 2048 + Threashold_Joy){
    //     Serial.println("Left");
    // } else if (Curr_Joy_X < 2048 - Threashold_Joy) {
    //     Serial.println("Right");
    // } else {
    //     // Serial.print("|         X Resting");
    // }

    // Serial.println(Move_X);
    // Serial.println(Move_Y);



    // Serial.println("");

    bleMouse.move(Move_X,-Move_Y,0);
    Click_Left_reg();
    Click_Right_reg();

}
