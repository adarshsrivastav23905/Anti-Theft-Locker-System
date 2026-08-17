# Simulation Test Scenarios

These 8 detailed scenarios define the exact behavior expected when running the system in the Wokwi environment.

## Scenario 1: Initial Boot-up
- **Preconditions:** System is powered off.
- **Steps to perform:** Start the simulation.
- **Expected observations:** 
  - OLED initializes and displays "Enter PIN:".
  - Servo moves to its default 0° position.
  - All LEDs and Buzzer remain OFF.
  - Serial monitor outputs "System Initialized. State: LOCKED".
- **Pass criteria:** Boot sequence completes under 3 seconds without crash loops.

## Scenario 2: Successful Authentication
- **Preconditions:** System is in `STATE_LOCKED`.
- **Steps to perform:** Press `1`, `2`, `3`, `4` on the keypad.
- **Expected observations:** 
  - OLED shows `****`.
  - Servo rotates to 90°.
  - Green LED turns ON.
  - OLED clears and displays "Access Granted / Unlocked".
- **Pass criteria:** Servo achieves exactly 90° and visual feedback matches state.

## Scenario 3: Auto-Lock Time Delay
- **Preconditions:** System is in `STATE_ACCESS_GRANTED`.
- **Steps to perform:** Do not press any buttons. Wait for 5 seconds.
- **Expected observations:** 
  - Servo rotates back to 0°.
  - Green LED turns OFF.
  - OLED reverts to "Enter PIN:".
- **Pass criteria:** Auto-lock executes exactly 5,000ms after unlock event.

## Scenario 4: Manual Lock Override
- **Preconditions:** System is in `STATE_ACCESS_GRANTED`.
- **Steps to perform:** Click the Pushbutton wired to GPIO5 before the 5-second timer expires.
- **Expected observations:** 
  - Servo immediately rotates to 0°.
  - Green LED turns OFF.
  - Auto-lock timer is cancelled/bypassed.
- **Pass criteria:** System secures itself instantly upon button press.

## Scenario 5: Incorrect PIN Handling
- **Preconditions:** System is in `STATE_LOCKED`. Failed attempts = 0.
- **Steps to perform:** Enter `8`, `8`, `8`, `8`.
- **Expected observations:** 
  - OLED displays "Access Denied / Wrong PIN".
  - Red LED flashes once.
  - Internal attempt counter increments to 1.
- **Pass criteria:** System does not unlock; input buffer clears completely for the next attempt.

## Scenario 6: Brute Force Alarm Activation
- **Preconditions:** System is in `STATE_LOCKED`. Failed attempts = 2.
- **Steps to perform:** Enter a third incorrect PIN (e.g., `9`, `9`, `9`, `9`).
- **Expected observations:** 
  - OLED displays "LOCKED OUT!".
  - Red LED turns ON solid.
  - Buzzer pulses On and Off repeatedly.
- **Pass criteria:** Alarm state is entered immediately upon the third failed digit string.

## Scenario 7: Input Denial During Lockout
- **Preconditions:** System is in `STATE_LOCKOUT`.
- **Steps to perform:** Rapidly click valid and invalid digits on the keypad.
- **Expected observations:** 
  - No `*` appears on the OLED.
  - Alarm continues to sound uninterrupted.
- **Pass criteria:** Keypad is 100% unresponsive to user input during the penalty phase.

## Scenario 8: Alarm Timeout & Reset
- **Preconditions:** System is in `STATE_LOCKOUT`.
- **Steps to perform:** Wait exactly 30 seconds without modifying power.
- **Expected observations:** 
  - Buzzer silences.
  - Red LED turns OFF.
  - System reverts to `STATE_LOCKED`.
  - Attempt counter resets to 0.
- **Pass criteria:** Entering `1234` immediately after the 30s timeout successfully opens the locker.
