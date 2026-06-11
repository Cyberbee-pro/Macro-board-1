#ifndef PINS_H
#define PINS_H 

#include <Arduino.h>
#include <ResponsiveAnalogRead.h>
#include <BleMouse.h>

extern float snapMultiplier;

extern BleMouse bleMouse;

// Row 1 (Top)
#define KEY_TOP_LEFT   21  // btn6
#define KEY_TOP_MID    22  // btn7
#define KEY_TOP_RIGHT  23  // btn1

// Row 2 (Middle)
#define KEY_MID_LEFT    5  // btn5
#define KEY_MID_MID    18  // btn4
#define KEY_MID_RIGHT  19  // btn8

// Row 3 (Bottom)
#define KEY_BOT_LEFT    4  // btn3
#define KEY_BOT_MID    16  // btn2
#define KEY_BOT_RIGHT  17  // btn9


#define BUTTON_TOGGLE_MACRO 14

// Leds
extern int LED[3];

//JoyStick
#define JOY_HOR 39
#define JOY_VERT 36
#define BUTTON_LEFT  27    // Joystick push button 
extern ResponsiveAnalogRead Joy_Hor_Res,Joy_Ver_Res;




#define BUTTON_RIGHT 26


//Potentiometer 
#define POT_SIG 35
extern ResponsiveAnalogRead Pot_Sig_Res;



extern int buttons[12];

void pins_setup();


#endif