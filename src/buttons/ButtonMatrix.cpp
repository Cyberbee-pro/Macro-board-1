#include <Arduino.h>
#include <BleKeyboard.h>
#include <BleMouse.h>
#include <debounce.h>
#include <PINS.h>
                
debounce buttonT1(&bleKeyboard,KEY_TOP_LEFT,KEY_ESC);
debounce buttonT2(&bleKeyboard,KEY_TOP_MID,'e');
debounce buttonT3(&bleKeyboard,KEY_TOP_RIGHT,KEY_LEFT_SHIFT);

debounce buttonM1(&bleKeyboard,KEY_MID_LEFT,KEY_TAB);
debounce buttonM2(&bleKeyboard,KEY_MID_MID,KEY_UP_ARROW);
debounce buttonM3(&bleKeyboard,KEY_MID_RIGHT,KEY_RETURN);

debounce buttonB1(&bleKeyboard,KEY_BOT_LEFT,KEY_LEFT_ARROW);
debounce buttonB2(&bleKeyboard,KEY_BOT_MID,KEY_DOWN_ARROW);
debounce buttonB3(&bleKeyboard,KEY_BOT_RIGHT,KEY_RIGHT_ARROW);


void (*call_mat)() = nullptr;


void run_matrix_def(){
    // Serial.println("\nDefault Matrix running");
    // Handle_T1();
    // Handle_T2();
    // Handle_T3();

    // Handle_M1();
    // Handle_M2();
    // Handle_M3();

    // Handle_B1();
    // Handle_B2();
    // Handle_B3();

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


void run_matrix(void (*input_profile)()){

    if(nullptr != input_profile){
        call_mat = input_profile;
    }

    if(nullptr != call_mat){
        call_mat();
    }else{
        run_matrix_def();
    }
}


// consider those as debug codes
// void Handle_T1(){
//     if(LOW == digitalRead(KEY_TOP_LEFT)){
//         Serial.println("T1");
//     }

    

// }
// void Handle_T2(){
//         if(LOW == digitalRead(KEY_TOP_MID)){
//         Serial.println("T2");
//     }
// }
// void Handle_T3(){
//         if(LOW == digitalRead(KEY_TOP_RIGHT)){
//         Serial.println("T3");
//     }
// }

// void Handle_M1(){
//     if(LOW == digitalRead(KEY_MID_LEFT)){
//         Serial.println("M1");
//     }
// }
// void Handle_M2(){
//         if(LOW == digitalRead(KEY_MID_MID)){
//         Serial.println("M2");
//     }
// }
// void Handle_M3(){
//     if(LOW == digitalRead(KEY_MID_RIGHT)){
//         Serial.println("M3");
//     }
// }

// void Handle_B1(){
//     if(LOW == digitalRead(KEY_BOT_LEFT)){
//         Serial.println("B1");
//     }
// }
// void Handle_B2(){
//     if(LOW == digitalRead(KEY_BOT_MID)){
//         Serial.println("B2");
//     }
// }
// void Handle_B3(){
//     if(LOW == digitalRead(KEY_BOT_RIGHT)){
//         Serial.println("B3");
//     }
// }
