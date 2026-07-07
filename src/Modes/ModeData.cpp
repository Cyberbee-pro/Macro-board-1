#include <Arduino.h>
#include <PINS.h>
#include <POT.h>
#include <JOY.h>
#include <BleKeyboard.h>
#include <BleMouse.h>
#include <Butt_matrix.h>
#include <ModeDisp.h>
#include <debounce.h>
#include <VirtualButton.h>



namespace GamingProfile {
    // These are safely isolated from the default layout in ButtonMatrix.cpp
    debounce buttonT1(&bleKeyboard, KEY_TOP_LEFT, KEY_ESC);
    debounce buttonT2(&bleKeyboard, KEY_TOP_MID, 'q'); 
    debounce buttonT3(&bleKeyboard, KEY_TOP_RIGHT, KEY_LEFT_SHIFT);

    debounce buttonM1(&bleKeyboard, KEY_MID_LEFT, KEY_TAB);
    debounce buttonM2(&bleKeyboard, KEY_MID_MID, 'w');
    debounce buttonM3(&bleKeyboard, KEY_MID_RIGHT, ' ');

    debounce buttonB1(&bleKeyboard, KEY_BOT_LEFT, 'a');
    debounce buttonB2(&bleKeyboard, KEY_BOT_MID, 's');
    debounce buttonB3(&bleKeyboard, KEY_BOT_RIGHT, 'd');

    void run_layout() {
        buttonT1.run_process();
        buttonT2.run_process();
        buttonT3.run_process();

        buttonM1.run_process();
        buttonM2.run_process();
        buttonM3.run_process();

        buttonB1.run_process();
        buttonB2.run_process();
        buttonB3.run_process();
    }
}



namespace profile_gaming_pot{
    debounce dl(&bleKeyboard,BUTTON_LEFT,KEY_RETURN);
    debounce dr(&bleMouse,BUTTON_RIGHT,KEY_LEFT_CTRL);

    VirtualButton vUp(KEY_UP_ARROW);
    VirtualButton vDown(KEY_DOWN_ARROW);
    VirtualButton vLeft(KEY_LEFT_ARROW);
    VirtualButton vRight(KEY_RIGHT_ARROW);


    void Click_Left_reg(){
        dl.run_process();
    }

    void Click_Right_reg(){
        dr.run_process();
    }

    void joy_run_gme2(){
    vUp.update(Move_Y >= 1);
    vDown.update(Move_Y <= -1);
        
    vRight.update(Move_X >= 1);
    vLeft.update(Move_X <= -1);
    
    Click_Left_reg();
    Click_Right_reg();
    }


}




void run_def(){
    joy_state_update(nullptr);
    pot_update(nullptr);
    run_matrix(nullptr);
}

void run_gaming(){
    joy_state_update(nullptr);
    pot_update(nullptr);
    run_matrix(GamingProfile::run_layout);
}

void run_game2(){
    joy_state_update(profile_gaming_pot::joy_run_gme2);
    pot_update(nullptr);
    run_matrix(GamingProfile::run_layout);
}


// Set up the static member tracker initial value
int mode::count = 1;

mode def("Default", &run_def);
mode gaming("Gaming", &run_gaming);
mode gameing2("gaming2", &run_game2);

// Global vector layout initialized as an empty shell
std::vector<mode*> mode_list_vec;

// Populated sequentially during setup stage
void populate_modes() {
    mode_list_vec.clear();
    mode_list_vec.push_back(&def);
    mode_list_vec.push_back(&gaming);
    mode_list_vec.push_back(&gameing2);
}


