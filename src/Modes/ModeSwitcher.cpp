#include <Arduino.h>
#include <PINS.h>
#include <POT.h>
#include <ModeDisp.h>


bool Current_Butt_state;
bool Prev_Butt_state = HIGH;

mode *Current_config = &def;
int curr_srl = 1;

void SearchNset(int target_srl){
    // Fallback if target is out of bounds
    if(target_srl < 1 || target_srl > 3) {
        Current_config = &def;
        curr_srl = 1;
        return;
    }

    for(mode* temp : mode_list_vec){
        if(temp != nullptr && temp->getSerial() == target_srl){
            Current_config = temp;
            curr_srl = target_srl;
            return;
        }
    }
}

void Mode_switch(){
    int maxCount = mode::getCount() - 1; 
    if (maxCount <= 0) maxCount = 3; // Static count recovery fallback

    Current_Butt_state = digitalRead(BUTTON_TOGGLE_MACRO);

    if(Current_Butt_state == LOW && Prev_Butt_state == HIGH){
        Serial.println("Macro Button pressed");
        
        setCurSrl();
        if(curr_srl < 1 || curr_srl > 3) curr_srl = 1;

        if(curr_srl < maxCount){
            SearchNset(curr_srl + 1);
        } else {
            SearchNset(1);
        }

        setCurSrl();
        if(curr_srl < 1 || curr_srl > 3) curr_srl = 1;

        Serial.printf("\n||Current Macro : %s ||Serial : %d\n", 
                      Current_config->getName().c_str(), curr_srl);
        delay(150); // Debounce window
    }
    Prev_Butt_state = Current_Butt_state;
}

void Mode_run(){
    if (Current_config != nullptr) {
        Current_config->call();
    }
}

