#include <Arduino.h>
#include <PINS.h>
#include <JOY.h>
#include <debounce.h>

debounce dl(&bleMouse,BUTTON_LEFT,MOUSE_LEFT);
debounce dr(&bleMouse,BUTTON_RIGHT,MOUSE_RIGHT);


void Click_Left_reg(){
    dl.run_process();
}


void Click_Right_reg(){
    dr.run_process();
}



