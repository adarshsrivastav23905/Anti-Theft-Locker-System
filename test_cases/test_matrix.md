# 🧪 System Verification Test Matrix

| Test Case ID | Test Scenario | Input Condition | Expected System Behavior | Verification Status |
|---|---|---|---|---|
| **TC-01** | System Initialization | Power-on / Hardware Reset | OLED displays Splash Screen, Servo locks ($0^\circ$), Red/Green LEDs OFF | **PASSED** |
| **TC-02** | Correct Password Input | Keypad: `1234#` | Servo opens ($90^\circ$), Green LED ON, OLED shows "ACCESS GRANTED", Double Beep | **PASSED** |
| **TC-03** | Auto-Lock Timeout | Wait 5 seconds after TC-02 | Servo re-locks ($0^\circ$), Green LED OFF, OLED returns to "LOCKED" screen | **PASSED** |
| **TC-04** | Single Incorrect Password | Keypad: `1111#` | Red LED turns ON, Buzzer beeps once, OLED shows "ACCESS DENIED (Attempts left: 2)" | **PASSED** |
| **TC-05** | Keypad Clear Operation | Keypad: `12*` | Password buffer resets, OLED screen clears typed asterisks `[____]` | **PASSED** |
| **TC-06** | Second Incorrect Password | Keypad: `2222#` | Red LED ON, OLED shows "ACCESS DENIED (Attempts left: 1)" | **PASSED** |
| **TC-07** | Brute-Force Alarm Trigger | Keypad: `3333#` (3rd failure) | Alarm activates: Pulsing siren, Red flashing LED, OLED shows "!! INTRUDER !!" | **PASSED** |
| **TC-08** | Keypad Lockout Behavior | Press keys during Alarm state | Keypad inputs ignored during 30-second lockout period | **PASSED** |
| **TC-09** | Lockout Expiration | Wait 30 seconds after TC-07 | Alarm stops, failed attempts reset to 0, system returns to normal armed state | **PASSED** |
| **TC-10** | Manual Lock Button | Press GPIO 5 Push Button | Servo locks immediately ($0^\circ$), alarm resets, screen returns to armed view | **PASSED** |
| **TC-11** | Password Masking Check | Type `1234` | OLED displays `[****]` obscuring actual digits | **PASSED** |
| **TC-12** | NVS Persistence Check | Power cycle device after TC-06 | System retains failed attempt count across reboots | **PASSED** |
| **TC-13** | Rapid Key Input Test | Press keys rapidly | Software debouncing prevents key skipping or double registers | **PASSED** |
| **TC-14** | Serial Telemetry Audit | Monitor UART at 115200 baud | Structured telemetry logs emitted every 2 seconds matching state transitions | **PASSED** |
| **TC-15** | Incomplete Password Submit | Keypad: `12#` | Fails authentication, registers as invalid password attempt | **PASSED** |
