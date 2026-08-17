# Hardware Components

| Component | Qty | Purpose | Input/Output Role | Expected Behavior | Key Specification |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **ESP32 DevKit V4** | 1 | Main controller, FSM execution, NVS storage | Processing Core | Runs code, manages I/O, controls timing | Xtensa Dual-Core 32-bit, 240MHz, 3.3V Logic |
| **4x4 Matrix Keypad** | 1 | User interface for PIN entry | Input (8 Pins) | Registers button presses (0-9, A-D, *, #) | Membrane/Push-button, requires pull-ups |
| **SSD1306 OLED** | 1 | Visual interface, status display, masking | Output (I2C) | Displays prompts, asterisks, statuses | 128x64 Pixels, Monochrome, I2C (0x3C) |
| **SG90 Servo Motor** | 1 | Physical locking mechanism actuator | Output (PWM) | Rotates to 0° (Lock) or 90° (Unlock) | 50Hz PWM, 4.8V-6V operating voltage |
| **Active Buzzer** | 1 | Auditory alarm and feedback | Output (Digital) | Emits loud tone when HIGH (pulsed 1.66Hz) | 5V DC, internal oscillator |
| **Red LED** | 1 | Visual indicator for denial/tampering | Output (Digital) | Illuminates on wrong pass / flashes in alarm | 5mm, ~2.0V Forward Voltage |
| **Green LED** | 1 | Visual indicator for granted access | Output (Digital) | Illuminates when locker opens successfully | 5mm, ~3.0V Forward Voltage |
| **Push Button** | 1 | Manual interior lock/reset trigger | Input (Digital) | Pulls GPIO5 to GND when pressed | Tactile switch, Normally Open (NO) |
| **Resistors** | 2 | Current limiting for LEDs | Passive Component | Prevents LEDs from drawing excess current | 330Ω (Ohm), 1/4 Watt |
| **Breadboard & Wires**| 1 | Prototyping interconnects | Infrastructure | Routes power and signals securely | Standard half/full-size, jumper wires (M-M) |
