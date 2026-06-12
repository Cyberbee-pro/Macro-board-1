#include <Arduino.h>


#include <PINS.h>
#include <POT.h>
#include <JOY.h>
#include <Butt_matrix.h>
#include <ModeDisp.h>
#include <dec2bin.h>



void run_def(){
    // Serial.println("Running default config - 1");
    joy_state_update();
    pot_update();
    run_matrix();
}

void run_gaming(){
    // Serial.println("Running gaming config - 2");
    joy_state_update();
    pot_update();
    run_matrix();
}

void run_media(){
    // Serial.println("Running Media config - 3");
    joy_state_update();
    pot_update();
    run_matrix();
}

// void run_midi(){
//     // Serial.println("Running Media config - 3");
//     joy_state_update();

// }



mode def("Default",&run_def);

mode gaming("Gaming",&run_gaming);

mode media("Media",&run_media);

// mode midi("Midi",&run_midi);


std::vector<mode*> mode_list_vec = {&def,&gaming,&media};
