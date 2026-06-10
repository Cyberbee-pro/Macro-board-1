#include <Arduino.h>
#include <string>
#include <vector>


#include <PINS.h>
#include <POT.h>
#include <ModeDisp.h>
#include <dec2bin.h>



void run_def(){
    // Serial.println("Running default config - 1");
    pot_update();
}

void run_gaming(){
    // Serial.println("Running gaming config - 2");
}

void run_media(){
    // Serial.println("Running Media config - 3");
}



mode def("Default",&run_def);

mode gaming("Gaming",&run_gaming);

mode media("Media",&run_media);

std::vector<mode*> mode_list_vec = {&def,&gaming,&media};
