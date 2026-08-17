# Security Logic & Architecture

This document details the defensive and security-focused algorithms implemented in the Anti-Theft Locker System.

## Password Storage: Hardcoded vs. NVS
In basic projects, passwords are often hardcoded (`String pass = "1234";`). If the device loses power, it simply reverts to this compiled string. 
In a real-world scenario, users need to update their pins dynamically. The ESP32 utilizes the **Preferences API** to interact with Non-Volatile Storage (NVS). 
- **NVS vs EEPROM:** Traditional Arduinos use EEPROM, which writes byte-by-byte and has strict read/write lifecycle limits (wear-out). The ESP32 does not have dedicated EEPROM; it uses a portion of its Flash memory. The NVS API abstracts wear-leveling, string handling, and sector erasure, making it vastly safer and more robust than raw EEPROM manipulation.

## Anti-Brute-Force Lockout Rules
To prevent an attacker from systematically guessing 10,000 combinations (0000-9999), the system utilizes strict threshold logic.

1. **Attempt Tracking:** An integer `failedAttempts` tracks bad entries.
2. **Threshold:** If `failedAttempts == 3`, the system immediately escalates to `STATE_LOCKOUT`.
3. **Total Blockade:** During `STATE_LOCKOUT`, `keypad.getKey()` inputs are intentionally ignored.
4. **Active Alarm:** Visual (Red LED) and auditory (Buzzer) indicators trigger immediately.
5. **Reset Condition:** The lockout only expires when time elapsed exceeds 30,000ms. Disconnecting and reconnecting power does not bypass this if state memory is configured aggressively (though currently, a power cycle resets volatile RAM counters, a known limitation).

## Lockout Timing Calculation
The lockout leverages hardware counters via `millis()`:
```cpp
unsigned long lockoutStartTime = millis();
const unsigned long LOCKOUT_DURATION = 30000; // 30 seconds

// Inside main loop during STATE_LOCKOUT:
if (millis() - lockoutStartTime >= LOCKOUT_DURATION) {
    failedAttempts = 0;
    currentState = STATE_LOCKED;
    // Turn off alarms
}
```
This ensures precise 30-second delays without pausing the MCU's ability to maintain the buzzer's PWM/pulsation.

## Auto-Lock Security Mechanism
Human error (forgetting to lock the safe) is a primary physical security flaw. 
Once access is granted (`STATE_ACCESS_GRANTED`), an internal timer begins. After exactly 5000ms, the system forcibly transitions to `STATE_LOCKED`, actuates the servo to 0°, and turns off the Green LED. This guarantees the vault is never left compromised indefinitely.

## Password Masking Logic
To prevent "shoulder surfing" (bystanders watching the screen as the PIN is entered), the actual keystrokes are stored in memory but replaced with asterisks (`*`) on the I2C OLED display.

*Code snippet abstraction:*
```cpp
String displayMask = "";
for(int i = 0; i < inputBuffer.length(); i++) {
    displayMask += "*";
}
display.print(displayMask);
```

## State Rules Summary
- **Correct Attempt:** Counter resets to 0, Servo -> 90°, Green LED ON, State -> GRANTED.
- **Wrong Attempt (1st/2nd):** Counter increments (+1), Buffer clears, Red LED blinks, OLED warns "Wrong PIN", State returns to LOCKED.
- **Wrong Attempt (3rd):** Counter hits 3. Servo stays 0°, Red LED solid, Buzzer sirens, State -> LOCKOUT. Screen shows "LOCKED OUT!".

## Display Security 
At no point is the `storedPassword` or the raw `inputBuffer` transmitted over UART telemetry or printed to the OLED in cleartext. Debug serial logs mask the input to maintain data hygiene even during diagnostic monitoring.
