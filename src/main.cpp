#include <Arduino.h>
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

#define BUTTON_RIGHT 26
#define BUTTON_LEFT  27    // Joystick push button 

#define BUTTON_TOGGLE_MACRO 14

#define LED_UNO 25
#define LED_DOS 33
#define LED_TRESS 32

#define POT_SIG 35

#define JOY_HOR 35
#define JOY_VERT 35



// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}