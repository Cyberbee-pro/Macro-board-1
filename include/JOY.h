#ifndef JOY_H
#define JOY_H

#include <Arduino.h>
#include <PINS.h>

extern int Threashold_Joy;
extern int Curr_Joy_Y;
extern int Curr_Joy_X;
extern int Move_Y;
extern int Move_X;

void joy_state_update(void (*Call_inp)() = nullptr);

void joy_run();

void joy_run_def();

void Click_Left_reg();
void Click_Right_reg();


extern void (*call_joy)();


#endif
