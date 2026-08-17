# Embedded Concepts Utilized

| Embedded Concept | Specific Usage in Anti-Theft Locker System |
| :--- | :--- |
| **Microcontroller (MCU)** | The "brain" of the project; specifically the **ESP32 DevKit V4** with a dual-core Xtensa processor running at 240MHz, coordinating all inputs, logic, and outputs. |
| **GPIO (General Purpose I/O)** | Utilizing the ESP32 pins to interface with peripherals. e.g., GPIO2 and GPIO4 for LEDs, GPIO15 for the Buzzer. |
| **Digital Input** | Reading discrete states (HIGH/LOW). Used on GPIO5 for the Lock Button to detect manual lock/reset commands. |
| **Digital Output** | Driving pins HIGH or LOW to control components. Used on GPIO2 (Red LED), GPIO4 (Green LED), and GPIO15 (Buzzer). |
| **Matrix Keypad Scanning** | Multiplexing 16 keys using 8 pins (Rows: 13,12,14,27. Cols: 26,25,33,32) using drive-and-sense scanning via the `Keypad.h` library. |
| **PWM (Pulse Width Modulation)** | Generating a 50Hz square wave with variable duty cycles on GPIO18 to control the SG90 Servo motor (0° for locked, 90° for unlocked). |
| **I2C Protocol (Inter-Integrated Circuit)** | 2-wire serial communication used to drive the SSD1306 OLED display using SDA (GPIO21) and SCL (GPIO22) at address `0x3C`. |
| **Servo Motor Control** | Translating logical states (Locked/Unlocked) into physical mechanical actuation via the `ESP32Servo` library on GPIO18. |
| **Finite State Machine (FSM)** | Architectural pattern used to govern system logic. Defined states: `STATE_LOCKED`, `STATE_ENTERING_PASS`, `STATE_ACCESS_GRANTED`, etc. |
| **Non-blocking Timing (`millis()`)** | Using hardware timer counts instead of `delay()` to manage the 5000ms auto-lock, 30000ms lockout, and 300ms buzzer pulse without freezing the keypad. |
| **UART Serial Communication** | Asynchronous serial bus used to send telemetry (system state, inputs) to the PC at 115200 baud every 2 seconds for debugging. |
| **Non-Volatile Storage (NVS)** | Saving the 4-digit PIN (default "1234") to flash memory via the Preferences API so the password survives power cycles. |
| **Password Masking** | Security UI concept. When a user presses a key, the internal buffer stores the digit, but the OLED displays an asterisk (`*`) to prevent shoulder surfing. |
| **String Comparison** | Comparing the user's volatile input buffer (`String inputPass`) against the NVS stored password (`String storedPass`) to grant or deny access. |
| **Attempt Counter** | Security logic concept. An integer variable `failedAttempts` increments on wrong entry and resets to 0 upon correct entry or lockout expiration. |
| **Software Debouncing** | Ignoring rapid, spurious signal changes from mechanical buttons. Applied to the keypad (50ms) and lock button to register only clean presses. |
| **Active Buzzer Control** | Transducer logic. Sending a 5V DC signal to GPIO15 triggers a continuous tone; modulating it on/off creates the 1.66Hz siren effect. |
| **LED Indicators** | Visual status feedback. Green LED (GPIO4) illuminates upon authorization, Red LED (GPIO2) illuminates on denial or alarm state. |
| **INPUT_PULLUP** | Internal MCU resistor configuration. Used on GPIO5 (Lock button) and the Keypad columns to hold the line at 3.3V until grounded by a physical press. |
| **Threshold Logic** | Using a predefined limit (3 max attempts) as a trigger threshold to drastically alter system behavior (transition to `STATE_LOCKOUT`). |
| **Conditional Statements** | `if`, `else if`, `switch/case` structures used extensively in the `loop()` to evaluate sensor data against FSM conditions. |
| **Interrupts (Future Scope)** | (Conceptual roadmap). Replacing polling-based button reads with hardware interrupts (e.g., `attachInterrupt()`) for instant reaction to physical tampering. |
