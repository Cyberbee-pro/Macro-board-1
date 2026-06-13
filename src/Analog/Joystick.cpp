#include <Arduino.h>
#include <PINS.h>
#include <JOY.h>
#include <SensorDebug.h>

int Threashold_Joy_UP   = 3100; // Increased to handle raw hardware noise cushions
int Threashold_Joy_DOWN = 1200; // Increased to handle raw hardware noise cushions
int Sensi = 15; 


int Curr_Joy_Y = 0;
int Curr_Joy_X = 0;

int Move_Y = 0;
int Move_X = 0;

void (*call_joy)() = nullptr;

void joy_state_update(void (*call_inp)()){ 
    
    Joy_Ver_Res.update();
    Joy_Hor_Res.update();
        
    // Use the filtered value here so control movement matches the debug "processed" mode.
    Curr_Joy_Y = Joy_Ver_Res.getValue();
    Curr_Joy_X = Joy_Hor_Res.getValue();
    if(Curr_Joy_X < Threashold_Joy_DOWN || Curr_Joy_X > Threashold_Joy_UP || Curr_Joy_Y < Threashold_Joy_DOWN || Curr_Joy_Y > Threashold_Joy_UP  ){
    Move_X = map(Curr_Joy_X,0,4095,-Sensi,Sensi);
    Move_Y = map(Curr_Joy_Y,0,4095,-Sensi,Sensi);
    }else{
        Move_X = 0;
        Move_Y = 0;
    }

    if (call_inp != nullptr) {
        call_joy = call_inp;
    }

    if (!SENSOR_DEBUG_MODE) {
        joy_run();
    }
}

void joy_run(){
    if(nullptr != call_joy){
        call_joy();
    }else{
        joy_run_def();
    }
}

void joy_run_def(){
    // 🟢 ONLY send Bluetooth packets if there is active movement out of the dead-zone
    if (Move_X != 0 || Move_Y != 0) {
        bleMouse.move(Move_X, -Move_Y, 0);
    }
    
    Click_Left_reg();
    Click_Right_reg();
}
