#include <Arduino.h>
#include <PINS.h>
#include <JOY.h>
#include <BleMouse.h>



unsigned int Tracking_Start_L = 0;
bool Tracking_stat_L= false,hold_Engaged_L = false ;

unsigned int Tracking_Start_R = 0;
bool Tracking_stat_R = false,hold_Engaged_R = false ;


void Click_Left_reg(){
    if(!Tracking_stat_L&& digitalRead(BUTTON_LEFT)==LOW){
        // Serial.println("\nStarting process - Track - left_B");
        Tracking_stat_L= true;
        hold_Engaged_L = false;
        Tracking_Start_L = millis();

        bleMouse.click();
        // Serial.println("Left Button Clicked");
        delay(90);
    }else if(millis() - Tracking_Start_L > 200 && digitalRead(BUTTON_LEFT)==LOW && Tracking_stat_L){
        bleMouse.press();
        // Serial.println("Left Button Pressed");
        if(!hold_Engaged_L){
        hold_Engaged_L = true;
        }
    }else if(millis() - Tracking_Start_L > 500 && digitalRead(BUTTON_LEFT)==HIGH && hold_Engaged_L){
        bleMouse.release();
        Tracking_stat_L= false;
        hold_Engaged_L = false;
        // Serial.println("Left Button Released");
    }
}


void Click_Right_reg(){
    if(!Tracking_stat_R && digitalRead(BUTTON_RIGHT)==LOW){
        // Serial.println("\nStarting process - Track - Right_B");
        Tracking_stat_R= true;
        hold_Engaged_R = false;
        Tracking_Start_R = millis();

        bleMouse.click(MOUSE_RIGHT);
        // Serial.println("Right Button Clicked");
        delay(90);
    }else if(millis() - Tracking_Start_R > 200 && digitalRead(BUTTON_RIGHT)==LOW && Tracking_stat_R){
        bleMouse.press(MOUSE_RIGHT);
        // Serial.println("Right Button Pressed");
        if(!hold_Engaged_R){
        hold_Engaged_R = true;
        }
    }else if(millis() - Tracking_Start_R > 500 && digitalRead(BUTTON_RIGHT)==HIGH && hold_Engaged_R){
        bleMouse.release(MOUSE_RIGHT);
        Tracking_stat_R= false;
        hold_Engaged_R = false;
        // Serial.println("Right Button Released");
    }
    // if(digitalRead(BUTTON_RIGHT)==LOW){
    //     bleMouse.click(MOUSE_RIGHT);
    //     Serial.println("Right Button pressed");
    // }
}



