 Code Quality and Structural Analysis Report: Macroboard ESP32


  This report outlines the structural integrity and code quality of the Macroboard project, focusing on header management, variable usage, and
  architectural optimizations.

  ---


  1. Unused & Redundant Headers
  Redundant inclusions increase compilation time and indicate a lack of centralized dependency management.



  ┌───────────────────────────────┬───────────────────────────────────────────┬────────────────────────────────────────────────────────┐
  │ File Path                     │ Redundant / Unused Headers                │ Context                                                │
  ├───────────────────────────────┼───────────────────────────────────────────┼────────────────────────────────────────────────────────┤
  │ src/main.cpp                │ <BleMouse.h>, <BleKeyboard.h>         │ Already provided by PINS.h.                          │
  │ src/Analog/mouseButtons.cpp │ <BleMouse.h>, <BleKeyboard.h>,        │ PINS.h covers BLE headers; JOY.h is redundant as   │
  │                               │ JOY.h                                   │ implementation is local or circular.                   │
  │ src/Modes/ModeSwitcher.cpp  │ <BleKeyboard.h>, <string>, <vector> │ <string> and <vector> are already in ModeDisp.h. │
  │ src/Modes/ModeData.cpp      │ <string>, <vector>,                   │ Heavy overlap with ModeDisp.h and PINS.h.          │
  │                               │ <BleKeyboard.h>, dec2bin.h            │                                                        │
  │ include/JOY.h               │ include <JOY.h>                         │ Recursive inclusion (guarded, but unnecessary).        │
  │ src/Analog/Pot.cpp          │ <BleKeyboard.h>                         │ Redundant inclusion.                                   │
  └───────────────────────────────┴───────────────────────────────────────────┴────────────────────────────────────────────────────────┘

  ---


  2. Variable Usage Analysis
  Identification of unused, misused, or poorly scoped variables.



  ┌───────────────────────────────┬──────────────────────┬─────────────────────────────────────────────────────────────────────────────────────┐
  │ File Path                     │ Symbol               │ Finding                                                                             │
  ├───────────────────────────────┼──────────────────────┼─────────────────────────────────────────────────────────────────────────────────────┤
  │ src/Modes/ModeSwitcher.cpp  │ led_array_state    │ Global variable used as a temporary scratchpad in Mode_show_bin. Should be local. │
  │ src/Analog/Joystick.cpp     │ Threashold_Joy     │ Declared and initialized (150) but only used in commented-out debug code.         │
  │ src/Analog/Pot.cpp          │ prev_Time_stamp    │ Logic Error: Reset to 0 at the end of pot_update, bypassing the 40ms rate   │
  │                               │                      │ limit after the first run.                                                          │
  │ src/Analog/Pot.cpp          │ sensi_vol          │ Constant value (1) that could be a constexpr or #define to save RAM.          │
  │ include/PINS.h              │ buttons[12]        │ Extern declaration for a fixed configuration array; could be const to reside in   │
  │                               │                      │ Flash.                                                                              │
  │ src/Analog/mouseButtons.cpp │ Tracking_Start_L/R │ Used unsigned int instead of unsigned long. Risks overflow issues with          │
  │                               │                      │ millis().                                                                         │
  └───────────────────────────────┴──────────────────────┴─────────────────────────────────────────────────────────────────────────────────────┘



  ---

  3. Optimization Scopes
  Architectural inefficiencies and opportunities for performance/memory improvements.


  A. Blocking Calls (Critical)
  The use of delay() halts the entire execution loop, causing unresponsive inputs or stuttering in BLE HID reports.
   - `src/Modes/ModeSwitcher.cpp`: delay(200) during mode switching prevents simultaneous button presses or joystick movement.
   - `src/Analog/mouseButtons.cpp`: delay(90) during mouse clicks creates noticeable lag in cursor tracking.
   - Recommendation: Implement a non-blocking state machine using millis() timers.


  B. Data Type & Memory Efficiency
   - Timestamp Types: Throughout the project (e.g., mouseButtons.cpp), timing variables use unsigned int. While ESP32 int is 32-bit, standard
     practice for millis() is unsigned long to ensure architectural portability and clarity.
   - Global Variable Bleeding: Variables like Current_Butt_state and led_array_state are global but only relevant to specific logic blocks. Moving
     these to static or private class members would reduce namespace pollution.
   - Floating Point Usage: snapMultiplier in PINS.h is a float. If used in hot loops with ResponsiveAnalogRead, ensure it's necessary or consider
     fixed-point math if RAM/CPU pressure increases.


  C. Algorithmic Inefficiencies
   - Linear Search (`SearchNset`): Uses a for loop to find modes by ID. While negligible for 3 modes, using a std::map<int, mode*> or direct array
     indexing would be more idiomatic and scalable.
   - Redundant State Sync: Mode_show() and Mode_show_bin() call setCurSrl() repeatedly, which performs an indirect method call. The curr_srl value
     should be cached or passed as a parameter.


  D. Build Configuration
   - Compiler Flags: The platformio.ini lacks optimization flags. Adding build_flags = -Os would significantly reduce the Flash footprint by
     optimizing for size, which is critical for BLE-enabled ESP32 projects where the BT stack consumes significant memory.


  ---
  Summary of Impact: The codebase is functional but suffers from significant "Stop-the-world" latency due to delay() calls and minor memory
  inefficiencies. Centralizing headers in PINS.h and fixing the pot_update timestamp logic are the highest-priority maintenance tasks.
