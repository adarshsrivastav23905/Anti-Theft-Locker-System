# Project Report: Anti-Theft Locker System

**Author:** Adarsh Srivastav
**Degree:** Diploma in Computer Science Engineering
**Subject:** Embedded Systems (Final Project)

---

## Abstract
This project presents the design and implementation of an Anti-Theft Locker System utilizing an ESP32 microcontroller. The system replaces traditional mechanical keys with a secure 4-digit digital PIN entry using a 4x4 matrix keypad. It features an I2C OLED display for masked visual feedback, an SG90 servo motor for lock actuation, and a Non-Volatile Storage (NVS) module for robust password persistence. To counter brute-force attacks, a strict Finite State Machine (FSM) architecture enforces a 30-second system lockout with an active siren and LED indicators after three consecutive failed attempts. An automatic 5-second locking mechanism further mitigates human error. The system was validated using the PlatformIO framework and Wokwi simulation.

## 1. Introduction
The vulnerabilities of physical lock systems, such as lock-picking, key duplication, and the absence of active tampering alerts, necessitate digital alternatives. This project aims to build an electronic locker that relies on "something you know" (a PIN) rather than "something you have" (a key). By integrating active deterrence mechanisms like alarms and strict timing penalties, the system physically and algorithmically defends against unauthorized access.

## 2. Literature Review
Electronic locking systems have evolved significantly:
1. **Basic Microcontroller Systems:** Early Arduino-based systems utilized EEPROM for password storage. These suffered from wear-leveling limits and lacked multitasking capabilities due to `delay()` driven logic.
2. **Commercial Hotel Safes:** Standard models utilize numeric keypads and solenoid locks but rarely include active alarms or persistent lockout delays, leaving them vulnerable to sustained guessing or brute-force attacks.
3. **IoT Smart Locks:** Modern systems utilize WiFi/Bluetooth. While feature-rich, they often neglect core offline security states. This project bridges the gap by focusing heavily on offline FSM reliability and NVS robustness using the 32-bit ESP32 architecture.

## 3. System Design & Methodology
The project employs a hardware-software co-design strategy. The physical layer comprises the ESP32 interfacing with an I2C OLED (SSD1306), an 8-pin Matrix Keypad, a PWM-driven Servo, and digital indicators (Buzzer, LEDs).
The software layer is structured around a Finite State Machine (FSM) containing six states: `STATE_LOCKED`, `STATE_ENTERING_PASS`, `STATE_ACCESS_GRANTED`, `STATE_ACCESS_DENIED`, `STATE_LOCKOUT`, and `STATE_ALARM_ACTIVE`. Time management is handled strictly via non-blocking `millis()` polling, allowing parallel execution of keypad scanning and buzzer pulsing.

## 4. Implementation Details
- **Microcontroller:** ESP32 DevKit V4 programmed in C++ using the Arduino Framework within PlatformIO.
- **Input Matrix:** The 16-key matrix is resolved into 8 GPIO pins via column pull-ups and sequential row grounding.
- **Security Logic:** Passwords are saved via the `Preferences.h` API directly to the ESP32 Flash memory. When a user inputs a digit, the OLED displays a `*` to mask the entry. Three failed attempts transition the FSM into a 30-second lockout phase where inputs are disregarded.
- **Actuation:** The `ESP32Servo` library generates a 50Hz PWM signal; ~1ms width equates to 0° (locked), and ~2ms width equates to 90° (unlocked).

## 5. Results & Observations
The integrated system performed flawlessly in simulation. The FSM successfully prevented race conditions (e.g., trying to type a password while the alarm is ringing). The NVS successfully retained the "1234" PIN across simulated power cycles. The non-blocking timers allowed the UART telemetry to output system diagnostics every 2 seconds accurately without interrupting the 1.66Hz buzzer modulation.

## 6. Testing Summary
A formal test matrix comprising 15 distinct scenarios was executed (detailed in `test_report.md`). Key passed tests include:
- Successful unlock leading to an automatic 5-second re-lock.
- 30-second complete input rejection upon three failed attempts.
- Instant mechanical reset using the internal push button (GPIO5).
- Accurate `*` masking on the OLED for every keystroke.

## 7. Conclusion
The ESP32 Anti-Theft Locker System successfully demonstrates the integration of multiple embedded protocols (I2C, PWM, GPIO Multiplexing) into a cohesive security product. The reliance on a strictly defined FSM and non-blocking timers ensures enterprise-grade reliability compared to standard procedural Arduino code. The project meets all defined academic and technical requirements for an embedded systems final project.

## 8. Future Scope
Subsequent versions of this project will explore:
1. **IoT Connectivity:** Adding Wi-Fi alerts via MQTT when the system enters the lockout state.
2. **Solenoid Upgrade:** Replacing the small SG90 servo with a 12V heavy-duty solenoid lock driven by an IRLZ44N MOSFET.
3. **Multi-Factor Authentication (MFA):** Integrating an RC522 RFID module to require both a physical card and a digital PIN.

## 9. References
- Espressif Systems. (2023). ESP32 Technical Reference Manual.
- Adafruit Industries. (2022). SSD1306 OLED Display Datasheet.
- Arduino Framework Documentation (millis() timing paradigms).
