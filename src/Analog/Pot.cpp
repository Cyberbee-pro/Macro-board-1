#include <Arduino.h>

#include <POT.h>
#include <PINS.h>


int Threashold = 3;
int Prev_Pot_state = 0;
int Curr_Pot_state = 0;
#define Pot_state_diff() (abs(Prev_Pot_state-Curr_Pot_state))
void (*call)() = nullptr;

void pot_update(void (*call_inp)()){
    if (call_inp != nullptr) {
        call = call_inp;
    }
    Pot_Sig_Res.update();
    Curr_Pot_state = map(Pot_Sig_Res.getValue(),0,4095,0,100);
    if(Curr_Pot_state != Prev_Pot_state && Pot_state_diff() > Threashold){
        pot_run();
    }
    Prev_Pot_state = Curr_Pot_state;
}

void pot_run(){
    if(nullptr == call){
        Serial.println("Default call function");
        //TODO : Implement volume control 
    }else{
        Serial.println("Custom call function");
        call();
    }

}
