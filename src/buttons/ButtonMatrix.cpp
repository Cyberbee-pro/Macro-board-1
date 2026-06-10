#include <Arduino.h>

#include <PINS.h>



void (*call_mat)() = nullptr;


void Handle_T1(){
    if(LOW == digitalRead(KEY_TOP_LEFT)){
        Serial.println("T1");
    }
}
void Handle_T2(){
        if(LOW == digitalRead(KEY_TOP_MID)){
        Serial.println("T2");
    }
}
void Handle_T3(){
        if(LOW == digitalRead(KEY_TOP_RIGHT)){
        Serial.println("T3");
    }
}

void Handle_M1(){
    if(LOW == digitalRead(KEY_MID_LEFT)){
        Serial.println("M1");
    }
}
void Handle_M2(){
        if(LOW == digitalRead(KEY_MID_MID)){
        Serial.println("M2");
    }
}
void Handle_M3(){
    if(LOW == digitalRead(KEY_MID_RIGHT)){
        Serial.println("M3");
    }
}

void Handle_B1(){
    if(LOW == digitalRead(KEY_BOT_LEFT)){
        Serial.println("B1");
    }
}
void Handle_B2(){
    if(LOW == digitalRead(KEY_BOT_MID)){
        Serial.println("B2");
    }
}
void Handle_B3(){
    if(LOW == digitalRead(KEY_BOT_RIGHT)){
        Serial.println("B3");
    }
}


void run_matrix_def(){
    // Serial.println("\nDefault Matrix running");
    Handle_T1();
    Handle_T2();
    Handle_T3();

    Handle_M1();
    Handle_M2();
    Handle_M3();

    Handle_B1();
    Handle_B2();
    Handle_B3();
}


void run_matrix(void (*call_inp)()){
    if(nullptr != call_inp){
        call_mat = call_inp;
    }else{
        run_matrix_def();
    }
}

