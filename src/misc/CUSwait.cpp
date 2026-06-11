
#include <Arduino.h>
#include <CUSwait.h>


    custom_wait::custom_wait(int wait_time){
        this->wait_time = wait_time;
        this->isActive =false;
        this->start_time = 0;
    }

    void custom_wait::start(){
        isActive =true;
        start_time = millis();
    }

    void custom_wait::stop(){
        isActive = false;
    }

    void custom_wait::reset_en_time(){
        start_time = millis();
    } 

    bool custom_wait::GetIsActive(){
        return isActive;
    }

    bool custom_wait::is_Time_Enlapsed(){
        
        if(!isActive){
            return false;
    }

        if((millis() - start_time) > wait_time){
            reset_en_time();
            return true;
        }else{
        return false;
        }
    }
