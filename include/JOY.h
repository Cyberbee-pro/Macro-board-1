#ifndef JOY_H
#define JOY_H

#include <Arduino.h>
#include <PINS.h>
#include <JOY.h>

extern int Threashold_Joy;

void joy_state_update(void (*Call_inp)() = nullptr);

void joy_run();

void joy_run_def();

extern void (*call_joy)();


#endif