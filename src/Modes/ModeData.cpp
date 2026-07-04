#include <Arduino.h>
#include <PINS.h>
#include <POT.h>
#include <JOY.h>
#include <BleKeyboard.h>
#include <BleMouse.h>
#include <Butt_matrix.h>
#include <ModeDisp.h>
#include <debounce.h>


namespace GamingProfile {
    // These are safely isolated from the default layout in ButtonMatrix.cpp
    debounce GbuttonT1(&bleKeyboard, KEY_TOP_LEFT, KEY_ESC);
    debounce GbuttonT2(&bleKeyboard, KEY_TOP_MID, 'q'); 
    debounce GbuttonT3(&bleKeyboard, KEY_TOP_RIGHT, KEY_LEFT_SHIFT);

    debounce GbuttonM1(&bleKeyboard, KEY_MID_LEFT, KEY_TAB);
    debounce GbuttonM2(&bleKeyboard, KEY_MID_MID, 'w');
    debounce GbuttonM3(&bleKeyboard, KEY_MID_RIGHT, 'e');

    debounce GbuttonB1(&bleKeyboard, KEY_BOT_LEFT, 'a');
    debounce GbuttonB2(&bleKeyboard, KEY_BOT_MID, 's');
    debounce GbuttonB3(&bleKeyboard, KEY_BOT_RIGHT, 'd');

    void run_layout() {
        GbuttonT1.run_process();
        GbuttonT2.run_process();
        GbuttonT3.run_process();

        GbuttonM1.run_process();
        GbuttonM2.run_process();
        GbuttonM3.run_process();

        GbuttonB1.run_process();
        GbuttonB2.run_process();
        GbuttonB3.run_process();
    }
}








void run_def(){
    joy_state_update();
    pot_update();
    run_matrix(nullptr);
}

void run_gaming(){
    joy_state_update();
    pot_update();
    run_matrix(GamingProfile::run_layout);
}

void run_media(){
    joy_state_update();
    pot_update();
    run_matrix(nullptr);
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


