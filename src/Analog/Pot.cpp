#include <Arduino.h>

#include <CUSwait.h>
#include <POT.h>
#include <PINS.h>
#include <SensorDebug.h>


#define Pot_state_diff() (abs(Prev_Pot_state - Curr_Pot_state))
#define Pot_state_diff_no_abs() (Curr_Pot_state - Prev_Pot_state)
#define Pot_Knob_diff() (abs(Prev_Knob_state - Curr_Knob_state))

int Threashold = 5000;
int Prev_Pot_state = 0;
int Curr_Pot_state = 0;
int Pot_Raw_state = 0;
// unsigned long int prev_Time_stamp = 0;
int sensi_vol = 1;
int Curr_Knob_state = 0;
int Prev_Knob_state = 0;



void pot_setup(){

    Pot_Sig_Res.update();

    Prev_Pot_state = Pot_Sig_Res.getValue();
    Curr_Pot_state = Pot_Sig_Res.getValue();
    Pot_Raw_state =  Pot_Sig_Res.getRawValue();

    Curr_Knob_state = map(Curr_Pot_state,0,4095,0,100);
    Prev_Knob_state = map(Curr_Pot_state,0,4095,0,100);

}



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
    Pot_Raw_state = Pot_Sig_Res.getRawValue();
    // Map the filtered reading to a stable percentage for the active control profile.
    Curr_Pot_state = Pot_Sig_Res.getValue();

    if(Pot_state_diff() < Threashold){
        return;
    }
    Curr_Knob_state = map(Curr_Pot_state,0,175000,0,100);
    if (!SENSOR_DEBUG_MODE) {
        pot_run();
    }
    
    Prev_Knob_state = Curr_Knob_state;
    Prev_Pot_state = Curr_Pot_state;
}

void pot_run(){
    if(nullptr == call){
        // Serial.println("Default call function");
        if(Pot_state_diff_no_abs() > 0){
            for(int i = 0;i<(Pot_Knob_diff()/sensi_vol);i++){
                bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
            // Serial.println("Vol up");
            delay(50);
            }
        }else{
            for(int i = 0;i<(Pot_Knob_diff()/sensi_vol);i++){
                bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
            // Serial.println("Vol down");
            delay(50);
            }
        }

    }else{
        call();
    }

}
