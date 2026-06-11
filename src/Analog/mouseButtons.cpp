#include <Arduino.h>
#include <PINS.h>
#include <JOY.h>
#include <BleMouse.h>
#include <BleKeyboard.h>
#include <debounce.h>


unsigned int Tracking_Start_R = 0;
bool Tracking_stat_R = false,hold_Engaged_R = false ;

debounce dl(mouse,BUTTON_LEFT,MOUSE_LEFT);
debounce dr(mouse,BUTTON_RIGHT,MOUSE_RIGHT);


void Click_Left_reg(){
    dl.run_process();
}


void Click_Right_reg(){
    dr.run_process();
}



