#include <Arduino.h>
#include <PINS.h>
#include <POT.h>
#include <ModeDisp.h>

void Mode_show(){
    // Force active state boundary protection inside the running loop
    setCurSrl();
    if (curr_srl < 1 || curr_srl > 3) {
        curr_srl = 1; 
    }

    // Explicit clear loop to prevent index calculation traps
    digitalWrite(LED[0], LOW);
    digitalWrite(LED[1], LOW);
    digitalWrite(LED[2], LOW);

    // Map profile serials 1, 2, 3 cleanly to LED array offsets 0, 1, 2
    int Cur_LED_INDEX = curr_srl - 1;
    if (Cur_LED_INDEX >= 0 && Cur_LED_INDEX < 3) {
        digitalWrite(LED[Cur_LED_INDEX], HIGH);
    }
}

// Left blank for compilation layout compatibility
void Mode_show_bin(){
}