# System Requirements Specification (SRS)

## 1. Software Requirements
To build, modify, and simulate this project, the following software toolchain is required:

- **IDE:** Visual Studio Code (VS Code) v1.80+
- **Framework:** PlatformIO IDE Extension v3.3+ (Arduino Framework)
- **Alternative IDE:** Arduino IDE 2.3+ (if not using PlatformIO)
- **Simulation:** Wokwi Simulator (Web-based or VS Code Extension)
- **Version Control:** Git v2.30+

## 2. Hardware Requirements
If building the physical prototype, these exact specifications are necessary:

- **Microcontroller:** ESP32 Development Board (e.g., NodeMCU-32S or DevKit V4) - 3.3V logic.
- **Keypad:** 4x4 Membrane or Push-Button Matrix Keypad (8-pin interface).
- **Display:** 0.96" SSD1306 OLED Display (128x64 resolution, I2C interface, 4-pin).
- **Actuator:** SG90 Micro Servo Motor (4.8V-6.0V rating, 50Hz PWM).
- **Audio:** 5V Active Buzzer.
- **Indicators:** 1x Red LED (5mm), 1x Green LED (5mm).
- **Passive Components:** 2x 330Ω Resistors, 1x Tactile Push Button.
- **Power:** 5V 1A Micro-USB/USB-C power supply (for ESP32). *Note: For physical builds, powering the SG90 directly from the ESP32's 3.3V pin is not recommended under load.*

## 3. Library Dependencies
The `platformio.ini` file must include the following library versions to ensure compilation:

- `adafruit/Adafruit SSD1306 @ ^2.5.10`
- `adafruit/Adafruit GFX Library @ ^1.11.9`
- `chris--a/Keypad @ ^3.1.1`
- `madhephaestus/ESP32Servo @ ^3.0.5`
- Built-in ESP32 `Preferences.h` (No external download needed)

## 4. System / OS Requirements
For the developer machine running VS Code and compiling the C++ source:
- **OS:** Windows 10/11, macOS 12+, or Ubuntu 20.04+
- **RAM:** Minimum 4GB (8GB recommended for VS Code + PlatformIO C++ Intellisense)
- **Disk Space:** ~2GB free space (PlatformIO toolchains and ESP32 core files require significant storage)
- **USB:** USB 2.0/3.0 port for serial flashing (requires CP2102 or CH340 drivers depending on ESP32 variant).
