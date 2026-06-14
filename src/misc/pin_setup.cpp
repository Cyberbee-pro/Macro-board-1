#include <PINS.h>
#include <POT.h>

float snapMultiplier = 0.01;

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

BleMouse bleMouse("MacroBoard V1","Saa-labs",100);
BleKeyboard bleKeyboard("MacroBoard V1","Saa-labs",100);


void pins_setup(){

    bleKeyboard.begin();
    Serial.println("Macroboard V1: Advertising Started...");

    pinMode(JOY_HOR, INPUT);
    pinMode(JOY_VERT, INPUT);
    pinMode(POT_SIG, INPUT);

    // 🟢 Fix 1: Hardcode the loop bound to exactly 3 elements
    for(int i = 0 ; i < 3 ; i++ ){
        pinMode(LED[i], OUTPUT);
    }

    // 🟢 Fix 2: Hardcode the loop bound to exactly 12 elements
    for(int i = 0 ; i < 12 ; i++ ){
        pinMode(buttons[i], INPUT_PULLUP);
    }

    // Joystick setup
    Joy_Hor_Res.setAnalogResolution(4095);
    Joy_Ver_Res.setAnalogResolution(4095);

    // Potentiometer Setup
    Pot_Sig_Res.setAnalogResolution(4095);

    Pot_Sig_Res.update();
    pot_setup();

    wait_pot.start();
}
