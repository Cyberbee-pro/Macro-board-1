#ifndef POT_H
#define POT_H


extern int Prev_Pot_state;
extern int Curr_Pot_state;
extern int Pot_Raw_state;
extern int Threashold;


void pot_update(void (*call_inp)() = nullptr);

void pot_run();

extern void (*call)();


#endif
