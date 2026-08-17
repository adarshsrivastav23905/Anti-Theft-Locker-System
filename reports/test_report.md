# Formal Test Execution Report

## Test Environment
- **Platform:** Wokwi Hardware Simulator / ESP32 DevKit V4 physical prototype
- **IDE:** Visual Studio Code with PlatformIO
- **Framework:** Arduino core for ESP32
- **Test Methodology:** Black-box functional testing and state transition validation.

## Test Cases & Results

| Test ID | Scenario | Precondition | Execution Steps | Expected Result | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **TC01** | System Boot & Init | Unpowered | Connect power | OLED shows prompt, Servo moves to 0°, LEDs OFF | **PASS** |
| **TC02** | Keystroke Masking | System LOCKED | Press '1', '2' on keypad | OLED displays `**` | **PASS** |
| **TC03** | Correct Password | System LOCKED | Enter `1234` | Servo -> 90°, Green LED ON, OLED -> "Unlocked" | **PASS** |
| **TC04** | Auto-Lock Mechanism | System GRANTED | Wait 5 seconds | Servo -> 0°, Green LED OFF, System returns to LOCKED | **PASS** |
| **TC05** | Manual Lock Button | System GRANTED | Press button (GPIO5) | Servo -> 0°, Green LED OFF immediately (skips 5s delay) | **PASS** |
| **TC06** | Wrong Password (1st) | System LOCKED | Enter `1111` | OLED shows "Wrong PIN", Red LED blinks, Attempts=1 | **PASS** |
| **TC07** | Wrong Password (2nd) | System LOCKED | Enter `2222` | OLED shows "Wrong PIN", Red LED blinks, Attempts=2 | **PASS** |
| **TC08** | Lockout Trigger (3rd) | Attempts=2 | Enter `3333` | System enters LOCKOUT, attempts set to 3 | **PASS** |
| **TC09** | Alarm Activation | System LOCKOUT | Observe outputs | Red LED solid ON, Buzzer pulses 300ms ON/OFF | **PASS** |
| **TC10** | Keypad Denial | System LOCKOUT | Press any keys | Keys completely ignored, buffer does not fill | **PASS** |
| **TC11** | Lockout Expiration | System LOCKOUT | Wait 30 seconds | Alarm stops, attempts reset to 0, returns to LOCKED | **PASS** |
| **TC12** | Input Buffer Reset | System ENTERING | Enter `12` then wait / fail | Buffer clears upon failure/lock, next entry starts at char 0 | **PASS** |
| **TC13** | UART Telemetry | Any State | Open Serial Monitor | Prints system state and attempt count every 2 seconds | **PASS** |
| **TC14** | NVS Persistence | Power OFF | Cycle power, enter `1234`| System successfully unlocks (password retained in flash) | **PASS** |
| **TC15** | Non-blocking Concurrency| System LOCKOUT | Observe Serial Monitor | UART logs print smoothly while buzzer pulses simultaneously | **PASS** |

## Test Summary
- **Total Cases Executed:** 15
- **Passed:** 15
- **Failed:** 0
- **Defects Found:** None critical. (Note: Wokwi simulation occasionally experiences visual lag on the OLED rendering, but logical FSM execution remains accurate).

## Recommendations
The system is stable and fully functional according to the requirement specifications. Recommended to proceed to physical PCB fabrication.
