# Project Explanation: Anti-Theft Locker System

## Simple Explanation
The Anti-Theft Locker System is a smart electronic security vault designed to protect valuable items using digital authentication instead of a physical key. Users interact with the locker by entering a 4-digit PIN on a keypad, and a clear OLED screen provides visual feedback by masking the password and displaying system statuses like "Access Granted" or "Access Denied." To enhance user experience and safety, the system provides visual (LED) and auditory (Buzzer) cues corresponding to lock activity.

What makes this system truly secure is its active defense mechanism. If an unauthorized person tries to guess the password and fails three consecutive times, the system triggers a loud alarm and initiates a 30-second complete system lockout. During this period, the locker ignores all keypad inputs, thwarting brute-force attacks. Furthermore, to prevent users from accidentally leaving the locker open, the system features an automatic locking mechanism that secures the vault 5 seconds after a successful unlock.

## Technical Explanation
At the core of the system lies a Finite State Machine (FSM) running on an ESP32 microcontroller, which dictates the system's operational flow through six distinct states: `STATE_LOCKED`, `STATE_ENTERING_PASS`, `STATE_ACCESS_GRANTED`, `STATE_ACCESS_DENIED`, `STATE_LOCKOUT`, and `STATE_ALARM_ACTIVE`. The FSM ensures deterministic behavior and strict state transitions based on predefined inputs (like correct or incorrect passwords) and timer events. All time-dependent operations, such as the 5-second auto-lock, the 30-second lockout penalty, and the 300ms buzzer pulsation, are implemented using non-blocking `millis()` logic. This allows the ESP32 to continuously scan the keypad matrix and maintain UART telemetry without freezing execution.

Hardware peripherals are heavily utilized and managed via standard embedded protocols. The SSD1306 OLED display communicates via the I2C protocol (`SDA=GPIO21`, `SCL=GPIO22` at `0x3C`), minimizing the pin count while providing a rich 128x64 pixel interface. The SG90 servo motor, acting as the physical lock actuator, is driven by a 50Hz PWM signal generated on `GPIO18`, where a 0° pulse width indicates a locked position and a 90° pulse width signifies an unlocked position. The 4x4 matrix keypad is read using a drive-and-sense multiplexing technique across 8 GPIO pins (4 rows, 4 columns), with internal pull-ups enabled to detect logic LOW when a key is pressed.

Data persistence is handled by the ESP32's Non-Volatile Storage (NVS) using the Preferences API. This ensures that the user's password remains intact even if the device loses power. When the user enters the PIN, the system dynamically masks the characters on the OLED to prevent shoulder-surfing. If a wrong PIN is entered, an attempt counter increments. Reaching three incorrect attempts shifts the FSM into `STATE_LOCKOUT`, activating a 1.66Hz siren on the active buzzer (`GPIO15`) and illuminating the Red LED (`GPIO2`), actively deterring tampering while the system refuses further input.

## System Workflow Diagram

```text
[ Power ON / Reset ]
         |
         v
+------------------+
|   INITIALIZE     |--> Load NVS Password, Setup Pins, I2C OLED, PWM Servo
+------------------+
         |
         v
+------------------+
|  STATE_LOCKED    |<-----------------------------------+
+------------------+                                    |
         | (Keypad Input Detected)                      |
         v                                              |
+------------------+                                    |
| ENTERING_PASS    |--> Mask Input ('*') on OLED        |
+------------------+                                    |
         | (4 Digits Entered)                           |
         v                                              |
     [ Check ]                                          |
      /     \                                           |
  Correct   Wrong                                       |
    /         \                                         |
   v           v                                        |
+---------+  +---------+                                |
| GRANTED |  | DENIED  |--> Increment Failed Attempts   |
+---------+  +---------+                                |
   |            |                                       |
   |            v                                       |
   |        [ Attempts == 3? ]                          |
   |          /      \                                  |
   |        Yes       No -------------------------------+
   |        /                                           |
   v       v                                            |
[Unlock] +----------+                                   |
[Green ] | LOCKOUT  |--> Activate Red LED + Siren       |
[Auto  ] +----------+    Ignore Inputs for 30s          |
[Lock  ]      |                                         |
   |          | (30s Timer Expires)                     |
   +----------+-----------------------------------------+
```

## Problem It Solves
Traditional mechanical lockers rely on physical keys, which present significant security vulnerabilities. Physical keys can be lost, stolen, or duplicated easily using basic tools or clay molds. Furthermore, mechanical locks offer no active defense; an intruder can pick the lock or try multiple keys indefinitely without the locker notifying anyone or defending itself. There is also no logging or visual indication of tampering.

## How Electronic Lockers Improve Security
1.  **No Physical Artifact to Lose:** Security relies on memorized credentials (a PIN) rather than a physical key that can be stolen or duplicated.
2.  **Active Brute-Force Defense:** Limits attempts (e.g., 3 tries) and enforces a time penalty (30 seconds) while triggering an alarm, rendering brute-force guessing mathematically and practically impossible.
3.  **Auditory & Visual Deterrence:** Built-in sirens and flashing LEDs instantly draw attention to unauthorized tampering attempts, scaring off intruders.
4.  **Auto-Locking Mechanism:** Eradicates the human error of forgetting to lock the safe after retrieving items. The system automatically secures itself after 5 seconds.
5.  **Dynamic Updates & Persistence:** Passwords can be changed digitally without replacing the lock cylinder, and credentials persist through power outages via NVS memory.

## Keypad Authentication & Matrix Scanning
The 4x4 keypad contains 16 buttons but only requires 8 GPIO pins (4 rows, 4 columns). It works on a drive-and-sense matrix scanning principle. The ESP32 sets the 4 row pins as outputs and the 4 column pins as inputs with internal pull-up resistors (defaulting to logic HIGH). 
The scanning algorithm rapidly drives one row LOW at a time while keeping the others HIGH. It then reads the 4 column pins. If a button is pressed, it bridges the active (LOW) row with its respective column, pulling that column pin LOW. The intersection of the active row and the LOW column uniquely identifies the pressed key. Software debouncing (50ms) ensures that mechanical contact bounce isn't registered as multiple key presses.

## Wrong Attempt Alarm & FSM State Transitions
When an incorrect password is submitted, the system transitions to `STATE_ACCESS_DENIED`. An internal counter tracks failed attempts. If this counter is strictly less than 3, the system displays a warning, resets the input buffer, and returns to `STATE_LOCKED`. 
However, if the counter reaches 3, a strict FSM transition occurs into `STATE_LOCKOUT` (and simultaneously `STATE_ALARM_ACTIVE`). In this state, the main loop ignores all keypad inputs. A non-blocking `millis()` timer records the exact entry time. The buzzer begins pulsing (300ms ON, 300ms OFF) to simulate a siren, and the Red LED remains solid. Only after the 30,000ms penalty duration expires does the FSM reset the attempt counter, silence the alarm, and transition back to `STATE_LOCKED`.
