#include <Arduino.h>
#include <PINS.h>
#include <ModeDisp.h>
#include <dec2bin.h>
#include <string>
#include <vector>

#define setCurSrl() (curr_srl = Current_config->getSerial())

bool Current_Butt_state;
bool Prev_Butt_state = HIGH;
int led_array_state = 0;

int mode::count = 1;


void run_def(){
    Serial.println("Running default config - 1");
}

void run_gaming(){
    Serial.println("Running gaming config - 2");
}

void run_media(){
    Serial.println("Running Media config - 3");
}


mode def("Default",&run_def);

mode gaming("Gaming",&run_gaming);

mode media("Media",&run_media);

std::vector<mode*> mode_list_vec = {&def,&gaming,&media};
mode *Current_config = &def;
int curr_srl = 1;

// int prev_state

void SearchNset(int target_srl){
    bool found = false;
    for(mode* temp : mode_list_vec){
        if(temp->getSerial()==target_srl){
            Current_config = temp;
            curr_srl = setCurSrl();
            found = true;
            return;
        }
    }
    
    if(found!=true){
    Serial.println("Target Serial not found. . . . . . .");
    }

}




void Mode_switch(){

    setCurSrl();
    int maxCount = mode::getCount() - 1;
    Current_Butt_state = digitalRead(BUTTON_TOGGLE_MACRO);
    if(Current_Butt_state == LOW && Prev_Butt_state == HIGH){
        Serial.println("Macro Button presed");

        if(curr_srl < maxCount){
            SearchNset(curr_srl + 1);
        }else if(curr_srl == maxCount){
            SearchNset(1);
        }

        Serial.print("\n||Current Macro : ");
        Serial.print((Current_config->getName()).c_str());
        Serial.print("||Serial : ");
        Serial.print(Current_config->getSerial());
        Serial.println("");
        delay(200);
    }
    Prev_Butt_state = Current_Butt_state ;

}



void Mode_run(){
    Current_config->call();
}


void Mode_show(){
    for(int i = 0; i<3;i++){
        digitalWrite(LED[i],LOW);
    }
    curr_srl = setCurSrl()-1;
    digitalWrite(LED[curr_srl],HIGH);
}


void Mode_show_bin(){
    for(int i = 0; i<3;i++){
        digitalWrite(LED[i],LOW);
    }
    curr_srl = setCurSrl();
    led_array_state = dec2bin(curr_srl);
    for(int i = 2;led_array_state>0;i--){
        digitalWrite(LED[i],led_array_state%10);
        led_array_state/=10;
    }
}