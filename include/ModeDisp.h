#ifndef MODE_H
#define MODE_H

#include <Arduino.h>
#include <vector>

#include <PINS.h>
#include <string>
#include <map>


extern bool Current_Butt_state;
extern bool Prev_Butt_state;

class mode{
    
    private : 
    int serial;
    static int count;
    std::string Profile_Name;
    void (*Hook) ();

    public :
    mode(std::string Profile_Name,void (*Hook)()){
        this->serial = count;
        count++;
        this->Profile_Name = Profile_Name;
        this->Hook = Hook;
    }

    int getSerial(){
        return serial;
    }

    static int getCount(){
        return count;
    }

    std::string getName(){
        return Profile_Name;
    }

    void call(){
        // Serial.print("\nExecuting Mode");
        // Serial.print(Profile_Name.c_str());
        // Serial.print("\t\t Serial : ");
        // Serial.print(serial);
        // Serial.println("");

        if(nullptr != Hook){
            Hook();
        }

    }
};

extern mode def,gaming,media,midi;
extern std::vector<mode*> mode_list_vec;

void Mode_switch();

void SearchNset(int target_srl);

void Mode_run();

void Mode_show();

void Mode_show_bin();





#endif