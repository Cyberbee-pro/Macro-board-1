#include <Arduino.h>
#include <BleMouse.h>
#include <BleKeyboard.h>
#include <debounce.h>
#include <CUSwait.h>



debounce::debounce(BleMouse *target_mouse, uint8_t pin, byte butt_args):wait_time(80)
{
    this->pin = pin;
    this->target_mouse = target_mouse;
    this->target_keyboard = nullptr;
    this->butt_args = butt_args;
    this->hold_Engaged = false;
    this->Tracking_Status = false;
}

debounce::debounce(BleKeyboard *target_keyboard, uint8_t pin, byte butt_args):wait_time(80)
{
    this->pin = pin;
    this->target_mouse = nullptr;
    this->target_keyboard = target_keyboard;
    this->butt_args = butt_args;
    this->hold_Engaged = false;
    this->Tracking_Status = false;
}


void debounce::trigger_action()
{

    if (!Tracking_Status && digitalRead(pin) == LOW)
    {
        Tracking_Status = true;
        hold_Engaged = false;
        wait_time.start();


            if (nullptr != target_mouse)
            {
                target_mouse->click(butt_args);
            }
            else if (nullptr != target_keyboard)
            {
                target_keyboard->write(butt_args);
            }
            Serial.println("Button Clicked");
        delay(35);
        // if(!wait_time_small.is_Time_Enlapsed()){
        //     return;
        // }
    }
    else if ( wait_time.is_Time_Enlapsed() && digitalRead(pin) == LOW && Tracking_Status && !hold_Engaged)
    {
        if (nullptr != target_mouse)
        {
            target_mouse->press(butt_args);
        }
        else if (nullptr != target_keyboard)
        {
            target_keyboard->press(butt_args);
        }

        Serial.println("Button Pressed");
            hold_Engaged = true;
    }
    else if (digitalRead(pin) == HIGH && Tracking_Status)
    {

        if (nullptr != target_mouse)
        {
            target_mouse->release(butt_args);
        }
        else if (nullptr != target_keyboard)
        {
            target_keyboard->release(butt_args);
        }
        Tracking_Status = false;
        hold_Engaged = false;
        // Serial.println("Button Released");
    }
}




void debounce::run_process()
{
    trigger_action();
}



























// GARBAGE EWW



// void debounce::trigger_action()
// {

//     if (!Tracking_Status && digitalRead(pin) == LOW)
//     {
//         Tracking_Status = true;
//         hold_Engaged = false;
//         Tracking_Start = millis();

//             if (type == Mouse)
//             {
//                 bleMouse.click(butt_args);
//             }
//             else if (type == Keyboard)
//             {
//                 bleKeyboard.write(butt_args);
//             }
//             Serial.println("Button Clicked");
//         delay(90);
//     }
//     else if (millis() - Tracking_Start > 200 && digitalRead(pin) == LOW && Tracking_Status)
//     {
//         if (type == Mouse)
//         {
//             bleMouse.press(butt_args);
//         }
//         else if (type == Keyboard)
//         {
//             bleKeyboard.press(butt_args);
//         }

//         Serial.println("Button Pressed");
//         if (!hold_Engaged)
//         {
//             hold_Engaged = true;
//         }
//     }
//     else if (digitalRead(pin) == HIGH)
//     {

//         if (type == Mouse)
//         {
//             bleMouse.release(butt_args);
//         }
//         else if (type == Keyboard)
//         {
//             bleKeyboard.release(butt_args);
//         }
//         Tracking_Status = false;
//         hold_Engaged = false;
//         // Serial.println("Button Released");
//     }
// }

