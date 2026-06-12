#ifndef debounce_h
#define debounce_h

#include <Arduino.h>
#include <BleMouse.h>
#include <BleKeyboard.h>

#include <CUSwait.h>


// enum Input_Interface {Mouse,Keyboard};

// constexpr Input_Interface mouse = Input_Interface::Mouse;
// constexpr Input_Interface keyboard = Input_Interface::Keyboard;



class debounce{
    private : 
    uint8_t pin;
    byte butt_args;
    custom_wait wait_time;
    // custom_wait wait_time_small;
    bool Tracking_Status,hold_Engaged ;
    // Input_Interface type;
    BleMouse *target_mouse;
    BleKeyboard *target_keyboard;

    protected :
    void trigger_action();

    public :
    debounce(BleMouse *target_mouse, uint8_t pins,byte butt_args);
    debounce(BleKeyboard *target_keyboard, uint8_t pins,byte butt_args);

    void run_process();

};

#endif






// if the firmware is too big we should switch to unified class with composition 

// example


// Current itteration


// enum InputType { TYPE_MOUSE, TYPE_KEYBOARD };

// class DebounceInput {
// private:
//     int pin;
//     uint8_t input_code; // e.g., MOUSE_LEFT or KEY_A
//     InputType type;
//     unsigned long tracking_start;
//     bool Tracking_Statuse;
//     bool hold_engaged;

// public:
//     DebounceInput(int pin_num, uint8_t code, InputType t) 
//         : pin(pin_num), input_code(code), type(t), tracking_start(0), Tracking_Statuse(false), hold_engaged(false) {}

//     void update() {
//         // Core state machine logic runs here identically for both
//         // If a valid press is detected, execute the payload:
//         triggerAction(true);
//     }

//     void triggerAction(bool press) {
//         if (type == TYPE_MOUSE) {
//             if (press) Mouse.press(input_code);
//             else Mouse.release(input_code);
//         } else if (type == TYPE_KEYBOARD) {
//             if (press) Keyboard.press(input_code);
//             else Keyboard.release(input_code);
//         }
//     }
// };





// More simplified approach for noobs : 

// Garbage code
// class debounce{
//     private : 
//     int pin;
//     byte butt_args;
//     unsigned int Tracking_Start;
//     bool Tracking_Status,hold_Engaged ;

//     public :

//     debounce(byte butt_args);

//     virtual void isHeldDown();

//     virtual void isClicked();

//     virtual void Release();

// };



// class debounce_Mouse : protected debounce{
//     public :

//     debounce_Mouse(byte butt_args);

//     void isHeldDown();

//     void isClicked();

//     void Release();

// };



// class debounce_Keyboard : protected debounce{
//     public :

//     debounce_Keyboard(byte butt_args);

//     void isHeldDown();

//     void isClicked();

//     void Release();

// };
