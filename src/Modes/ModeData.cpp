#include <Arduino.h>
#include <PINS.h>
#include <POT.h>
#include <JOY.h>
#include <Butt_matrix.h>
#include <ModeDisp.h>

void run_def(){
    joy_state_update();
    pot_update();
    run_matrix();
}

void run_gaming(){
    joy_state_update();
    pot_update();
    run_matrix();
}

void run_media(){
    joy_state_update();
    pot_update();
    run_matrix();
}

// Set up the static member tracker initial value
int mode::count = 1;

mode def("Default", &run_def);
mode gaming("Gaming", &run_gaming);
mode media("Media", &run_media);

// Global vector layout initialized as an empty shell
std::vector<mode*> mode_list_vec;

// Populated sequentially during setup stage
void populate_modes() {
    mode_list_vec.clear();
    mode_list_vec.push_back(&def);
    mode_list_vec.push_back(&gaming);
    mode_list_vec.push_back(&media);
}