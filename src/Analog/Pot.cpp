#include <Arduino.h>

#include <CUSwait.h>
#include <POT.h>
#include <PINS.h>


int Threashold = 3;
int Prev_Pot_state = 0;
int Curr_Pot_state = 0;
// unsigned long int prev_Time_stamp = 0;
int sensi_vol = 1;
#define Pot_state_diff() (abs(Prev_Pot_state-Curr_Pot_state))
#define Pot_state_diff_no_abs() (Curr_Pot_state-Prev_Pot_state)
custom_wait wait_pot(40);

void (*call)() = nullptr;

void pot_update(void (*call_inp)()){
    if(!wait_pot.is_Time_Enlapsed()){
        return;
    }
    if (call_inp != nullptr) {
        call = call_inp;
    }
    Pot_Sig_Res.update();
    Curr_Pot_state = map(Pot_Sig_Res.getValue(),0,4095,0,100);
    if(Pot_state_diff() > Threashold){
        pot_run();
        Prev_Pot_state = Curr_Pot_state;
    }
}

void pot_run(){
    if(nullptr == call){
        Serial.println("Default call function");
        if(Pot_state_diff_no_abs() > 0){
            for(int i = 0;i<(Pot_state_diff()/sensi_vol);i++){
                bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
            Serial.println("Vol up");
            }
        }else{
            for(int i = 0;i<(Pot_state_diff()/sensi_vol);i++){
                bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
            Serial.println("Vol down");
            }
        }

    }else{
        Serial.println("Custom call function");
        call();
    }

    // prev_Time_stamp = 0;


}
