# Automation Logic & FSM Design

The Anti-Theft Locker System avoids procedural "spaghetti code" by utilizing a Finite State Machine (FSM). 

## FSM State Definitions

| State | Entry Condition | Exit Condition | Active Outputs |
| :--- | :--- | :--- | :--- |
| **STATE_LOCKED** | Boot up, Auto-lock timer expires, Lockout timer expires | Any keypad button pressed | Servo: 0°, LEDs: OFF, OLED: "Enter PIN" |
| **STATE_ENTERING_PASS** | Key pressed while LOCKED | 4th digit entered | OLED: Displays `*` for each press |
| **STATE_ACCESS_GRANTED** | PIN matches NVS storage | Auto-lock timer (5s) or Manual lock button pressed | Servo: 90°, Green LED: ON, OLED: "Unlocked" |
| **STATE_ACCESS_DENIED** | PIN mismatch (Attempts < 3) | 2-second delay expires | Red LED: Blinks, OLED: "Wrong PIN" |
| **STATE_LOCKOUT / ALARM** | Failed attempts == 3 | 30-second penalty timer expires | Servo: 0°, Red LED: ON, Buzzer: Pulsing |

## Complete Decision Table

This table dictates system behavior based on input combinations.

| Current State | Input / Event | Condition | Next State | Action Taken |
| :--- | :--- | :--- | :--- | :--- |
| LOCKED | Key Press | Input length < 4 | ENTERING_PASS | Buffer += key, screen updates |
| ENTERING_PASS| Key Press | Input length reaches 4, Match == TRUE | ACCESS_GRANTED | Reset attempts, Unlock servo |
| ENTERING_PASS| Key Press | Input length reaches 4, Match == FALSE, Fails < 2 | ACCESS_DENIED | Increment fails, show warning |
| ENTERING_PASS| Key Press | Input length reaches 4, Match == FALSE, Fails == 2 | LOCKOUT | Set fails=3, trigger alarm, start 30s timer |
| ACCESS_GRANTED| Timer Check | `millis() - unlockTime > 5000` | LOCKED | Lock servo, clear screen |
| ACCESS_GRANTED| Button Press| GPIO5 pulled LOW | LOCKED | Lock servo, clear screen |
| ACCESS_DENIED| Timer Check | Delay equivalent passed | LOCKED | Clear buffer, reset screen |
| LOCKOUT | Key Press | Any key | LOCKOUT | Ignored |
| LOCKOUT | Timer Check | `millis() - lockoutStart > 30000` | LOCKED | Reset fails to 0, silence alarm |

## State Transition Rules (Priority Hierarchy)

1. **Highest Priority - Lockout Expiration:** If the system is in `LOCKOUT`, the timer evaluation has absolute priority over any physical inputs.
2. **High Priority - Auto-Lock / Manual Lock:** If the system is `GRANTED`, timer evaluation and GPIO5 polling override any keypad inputs.
3. **Medium Priority - Keypad Processing:** In `LOCKED` or `ENTERING_PASS`, the keypad scanner captures inputs.
4. **Lowest Priority - Telemetry:** UART reporting runs in the background regardless of state, utilizing its own non-blocking timer.
