#include <Arduino.h>
#include <PINS.h>
#include <ResponsiveAnalogRead.h>

int LED[3] = { 32, 33, 25};

int buttons[12] = {
    KEY_TOP_LEFT,  KEY_TOP_MID,  KEY_TOP_RIGHT,
    KEY_MID_LEFT,  KEY_MID_MID,  KEY_MID_RIGHT,
    KEY_BOT_LEFT,  KEY_BOT_MID,  KEY_BOT_RIGHT,
    BUTTON_RIGHT,  BUTTON_LEFT,  BUTTON_TOGGLE_MACRO
};



//joystick
ResponsiveAnalogRead Joy_Hor_Res(JOY_HOR,true,snapMultiplier);
ResponsiveAnalogRead Joy_Ver_Res(JOY_VERT,true,snapMultiplier);

//Potentiometer
ResponsiveAnalogRead Pot_Sig_Res(POT_SIG,true,snapMultiplier);


float snapMultiplier = 0.01;

void pins_setup(){
    
    for(int i = 0 ; i < sizeof(LED)/sizeof(LED[0]) ; i++ ){
        pinMode(LED[i],OUTPUT);
    }

    for(int i = 0 ; i < sizeof(buttons)/sizeof(buttons[0]) ; i++ ){
        pinMode(buttons[i],INPUT_PULLUP);
    }
    //Joystick setup
    Joy_Hor_Res.setAnalogResolution(4095);
    Joy_Ver_Res.setAnalogResolution(4095);

    //Potentiometer Setup
    Pot_Sig_Res.setAnalogResolution(4095);

}
