# Implementation Phases

### Phase 1: Project Setup & Dependency Management
- **Objective:** Establish the development environment and foundational project structure.
- **Tasks:**
  - Initialize PlatformIO project for ESP32 DevKit V4.
  - Configure `platformio.ini` with serial monitor speed (115200).
  - Install dependencies: Adafruit SSD1306, Keypad, ESP32Servo.
- **Expected Output:** A compiling blank project with all libraries linked.
- **Common Mistakes:** Mismatched baud rates, missing Adafruit GFX dependency.
- **Verification:** Compile successfully (`pio run`); serial monitor outputs a test string.

### Phase 2: OLED Display Integration
- **Objective:** Establish visual output capability using the I2C bus.
- **Tasks:**
  - Wire SDA (21) and SCL (22) to the SSD1306.
  - Initialize `display.begin(SSD1306_SWITCHCAPVCC, 0x3C)`.
  - Write helper functions for displaying text `displayMessage()`.
- **Expected Output:** "System Booting..." visible on the physical/simulated screen.
- **Common Mistakes:** Wrong I2C address (e.g., 0x3D instead of 0x3C).
- **Verification:** Text appears centered and readable on the screen.

### Phase 3: Keypad Matrix Scanning
- **Objective:** Capture user input via the 4x4 matrix keypad.
- **Tasks:**
  - Map the 8 keypad pins to ESP32 GPIOs.
  - Define the 2D char array `keys[4][4]`.
  - Implement the `keypad.getKey()` non-blocking read loop.
- **Expected Output:** Pressing keys outputs the corresponding character to the Serial Monitor.
- **Common Mistakes:** Swapping row and column pin arrays.
- **Verification:** Pressing 'A' prints 'A' over UART; no phantom presses.

### Phase 4: Servo Motor Actuation
- **Objective:** Control the physical locking mechanism using PWM.
- **Tasks:**
  - Attach the SG90 servo to GPIO18 using `ESP32Servo` library.
  - Create `lockDoor()` (0 degrees) and `unlockDoor()` (90 degrees) functions.
- **Expected Output:** Servo sweeps between 0 and 90 degrees based on code triggers.
- **Common Mistakes:** Overdrawing current from the ESP32 3.3V pin (use 5V/VIN or external power for physical builds).
- **Verification:** Servo firmly hits the 0 and 90-degree stops without jitter.

### Phase 5: Core Authentication Logic
- **Objective:** Compare user input against a hardcoded password.
- **Tasks:**
  - Buffer keypad inputs into a `String` variable.
  - Display `*` on the OLED for each press.
  - Trigger evaluation when string length reaches 4.
- **Expected Output:** Correct PIN calls unlock function; wrong PIN clears buffer.
- **Common Mistakes:** Forgetting to clear the input buffer after evaluation.
- **Verification:** Entering "1234" turns the servo; entering "1111" resets input.

### Phase 6: Lockout & Alarm Logic
- **Objective:** Implement brute-force protection using attempt counters.
- **Tasks:**
  - Add `failedAttempts` counter.
  - Wire Active Buzzer (GPIO15) and Red LED (GPIO2).
  - Implement `millis()` timer for 30-second penalty phase.
- **Expected Output:** 3 wrong attempts triggers a 30s period where keypad is ignored and buzzer sounds.
- **Common Mistakes:** Using `delay(30000)`, which crashes the watchdog or freezes background tasks.
- **Verification:** Intentionally fail 3 times; verify keypad is unresponsive for exactly 30s.

### Phase 7: NVS Persistent Storage
- **Objective:** Save the password so it survives power cycles.
- **Tasks:**
  - Include `<Preferences.h>`.
  - On setup, read stored password; if empty, set default "1234".
  - (Optional) Add a master code to update the password in NVS.
- **Expected Output:** Password persists after rebooting the ESP32.
- **Common Mistakes:** Exceeding NVS string length limits or forgetting to call `preferences.end()`.
- **Verification:** Change PIN (if UI allows) or verify default, reboot, and authenticate again.

### Phase 8: LED Indicators & Button Reset
- **Objective:** Provide explicit hardware feedback and manual control.
- **Tasks:**
  - Wire Green LED (GPIO4). Update auth logic to turn it ON upon success.
  - Wire Push Button (GPIO5) with `INPUT_PULLUP`.
  - Add logic to lock the door manually when button is pressed.
- **Expected Output:** LEDs reflect system state; interior button locks door instantly.
- **Common Mistakes:** Floating pins on the button (forgetting pull-up).
- **Verification:** Green LED syncs with Servo=90; pressing button resets Servo=0.

### Phase 9: Finite State Machine (FSM) Integration
- **Objective:** Refactor procedural code into a robust FSM architecture.
- **Tasks:**
  - Define `enum SystemState { LOCKED, ENTERING, GRANTED, DENIED, LOCKOUT }`.
  - Wrap logic in a massive `switch(currentState)` block inside `loop()`.
- **Expected Output:** System behavior remains identical, but code is modular and bug-free.
- **Common Mistakes:** Missing `break;` statements in the switch-case.
- **Verification:** State transitions map exactly to the workflow diagram.

### Phase 10: Auto-Lock Security Mechanism
- **Objective:** Automatically secure the locker if left open.
- **Tasks:**
  - Capture `millis()` timestamp upon `STATE_ACCESS_GRANTED`.
  - Continuously check if `(millis() - unlockTime) > 5000`.
  - Transition back to `STATE_LOCKED` automatically.
- **Expected Output:** Locker secures itself 5 seconds after a valid entry.
- **Common Mistakes:** Timer overflow issues (not using `unsigned long`).
- **Verification:** Enter correct PIN, wait 5 seconds, watch Servo return to 0°.

### Phase 11: Code Optimization & Telemetry
- **Objective:** Clean up code and add debug logging.
- **Tasks:**
  - Add a non-blocking 2-second heartbeat UART log (`Serial.printf`).
  - Extract magic numbers (pin definitions, times) into `#define` macros.
- **Expected Output:** Clean, highly readable code with UART diagnostics.
- **Common Mistakes:** Spamming the serial monitor by omitting the 2-second timer limit.
- **Verification:** PC serial monitor receives state updates like "State: LOCKED, Attempts: 0" every 2s.

### Phase 12: Final Testing & Documentation
- **Objective:** Validate system against all edge cases.
- **Tasks:**
  - Run through `test_scenarios.md`.
  - Write all markdown documentation and reports.
- **Expected Output:** 100% test pass rate and complete GitHub repository.
- **Common Mistakes:** Skipping regression tests after refactoring.
- **Verification:** Peer review of project report and successful demo in Wokwi.
