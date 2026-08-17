# Interview Preparation Q&A

This document prepares you to defend this project in technical interviews.

## Core Questions

**Q1: Explain your project.**
**A1:** My project is an ESP32-based Anti-Theft Locker System designed to replace vulnerable mechanical locks. It uses a 4x4 matrix keypad for PIN entry, an I2C OLED for masked visual feedback, and a servo motor for actuation. The core software architecture is built on a Finite State Machine (FSM) utilizing non-blocking `millis()` timers. It features active security mechanisms like a 30-second brute-force lockout with a buzzer alarm and a 5-second auto-lock to prevent human error. 

**Q2: Why did you choose the ESP32 over an Arduino UNO?**
**A2:** I chose the ESP32 because of its 32-bit architecture and ample memory, making it highly suitable for future expansions like IoT or RTOS integration. It also has a native Preferences API for robust NVS (Non-Volatile Storage) flash memory handling, which is superior to the Arduino's limited EEPROM for saving passwords.

**Q3: How does the matrix keypad work without using 16 separate pins?**
**A3:** It uses multiplexing via a drive-and-sense technique. The 16 buttons are wired in a 4x4 grid requiring only 8 GPIO pins (4 rows, 4 columns). The microcontroller sequentially drives one row LOW while keeping others HIGH, then reads the columns (which use internal pull-up resistors). If a column reads LOW, the intersection identifies the pressed key.

**Q4: Explain what a Finite State Machine (FSM) is and why you used it.**
**A4:** An FSM is an architectural pattern where the system can only exist in one defined state at a time (e.g., `LOCKED`, `GRANTED`, `LOCKOUT`). I used it because security systems require highly deterministic behavior. It prevents bugs like the servo unlocking while the alarm is supposed to be ringing, keeping the logic modular and strictly controlled by explicit transition conditions.

**Q5: How did you implement time delays without freezing the system?**
**A5:** I avoided the `delay()` function, which blocks the CPU. Instead, I used non-blocking timing via the `millis()` function. By taking a hardware timestamp when an event occurs and continuously comparing the current time minus the timestamp against a threshold interval, the ESP32 can manage timers (like the 30s lockout) while simultaneously scanning the keypad and pulsing the buzzer.

**Q6: What is I2C and how is it used here?**
**A6:** I2C (Inter-Integrated Circuit) is a synchronous, multi-master, multi-slave packet switched, single-ended, serial communication bus. I used it to connect the SSD1306 OLED to the ESP32. It is highly efficient because it requires only two wires (SDA for data and SCL for clock) plus power, communicating via specific hex addresses (like 0x3C).

**Q7: How did you implement brute-force protection?**
**A7:** I implemented an integer counter that tracks consecutive failed attempts. If a wrong PIN is entered, the counter increments. If it reaches exactly 3, the FSM transitions to a `LOCKOUT` state. In this state, keypad inputs are explicitly ignored, and a `millis()` timer holds the system in lockdown while sounding an alarm for 30 seconds.

**Q8: What is PWM and how does it control the lock?**
**A8:** PWM stands for Pulse Width Modulation. It generates a square wave with a specific frequency (50Hz for standard servos) where the width of the "ON" pulse determines the position. The ESP32 sends a ~1ms pulse to rotate the SG90 servo to 0° (locking the mechanism) and a ~2ms pulse to move it to 90° (unlocking it).

**Q9: What happens to the password if the power goes out?**
**A9:** The password persists safely because it is stored in the ESP32's Non-Volatile Storage (NVS) using the Preferences API. Unlike volatile RAM, NVS writes data to a designated sector of the ESP32's onboard flash memory, allowing the system to recall the user's custom PIN upon reboot.

**Q10: How do you handle hardware bouncing on the buttons?**
**A10:** Mechanical buttons physically vibrate (bounce) when pressed, generating multiple rapid HIGH/LOW signals that the fast MCU might read as multiple presses. I handled this using software debouncing, specifically relying on the `Keypad.h` library's built-in 50ms debounce timer, which ignores secondary state changes immediately following an initial press.

## Bonus Questions

**Q11: How would you upgrade this project for IoT?**
**A11:** I would utilize the ESP32's native Wi-Fi capabilities to connect to an MQTT broker. I could then send telemetry data to a cloud dashboard (like AWS or Blynk) to alert the user via push notifications if the alarm state is triggered.

**Q12: What is a Pull-Up resistor and where is it used?**
**A12:** A pull-up resistor connects a pin to the logic HIGH voltage (3.3V) to prevent it from floating randomly. I used the ESP32's internal pull-up resistors (`INPUT_PULLUP`) on the keypad columns and the manual lock button, so they read HIGH by default and cleanly drop to LOW when pushed.

**Q13: Why use an Active Buzzer over a Passive Buzzer?**
**A13:** An active buzzer has an internal oscillator; it just requires a DC voltage (HIGH) to produce a loud, specific tone, which is simple to trigger for an alarm. A passive buzzer requires the MCU to generate an audio frequency AC signal, adding unnecessary CPU overhead for a simple siren.

**Q14: Describe a critical bug you faced and how you solved it.**
**A14:** *(Example Answer)* Initially, the locker would stay open forever if the user forgot to lock it. I realized this defeated the security purpose. I solved this by adding an auto-lock feature that captures a `millis()` timestamp upon opening and forces the FSM back to the locked state after 5 seconds.

**Q15: How did you verify your code without risking physical hardware damage?**
**A15:** I built and verified the entire architecture using the Wokwi hardware simulator. This allowed me to test my FSM logic, wiring mapped to GPIO pins, and I2C/PWM outputs safely in a virtual environment before committing to a physical PCB or breadboard build.
