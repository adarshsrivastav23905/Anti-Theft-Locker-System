# Wokwi Execution Guide

Follow these 17 steps to accurately simulate and test the Anti-Theft Locker System in the browser using Wokwi.

## Part 1: Project Setup
1. **Open Wokwi:** Navigate to [wokwi.com](https://wokwi.com/) in your web browser.
2. **Create Project:** Click on "ESP32" to create a new blank project.
3. **Add Library Dependencies:** Go to the `Library Manager` tab in Wokwi and click the `+` icon. Search for and add:
   - `Adafruit SSD1306`
   - `Adafruit GFX Library`
   - `Keypad`
   - `ESP32Servo`
4. **Add Components:** Click the `+` icon in the simulation canvas and add the following:
   - 1x Membrane Keypad 4x4
   - 1x SSD1306 OLED Display (I2C)
   - 1x Servo (Standard)
   - 1x Buzzer
   - 2x LEDs (Change one to Red, one to Green)
   - 2x Resistors (330 ohms)
   - 1x Pushbutton

## Part 2: Wiring & Code Insertion
5. **Wire the Keypad:** Connect rows 1-4 to GPIOs 13, 12, 14, 27. Connect columns 1-4 to GPIOs 26, 25, 33, 32.
6. **Wire the OLED:** Connect VCC to 3.3V, GND to GND, SDA to GPIO21, and SCL to GPIO22.
7. **Wire the Peripherals:**
   - Servo PWM to GPIO18 (VCC to 5V).
   - Buzzer to GPIO15 (and GND).
   - Red LED anode to GPIO2 (via resistor), Green LED anode to GPIO4 (via resistor).
   - Pushbutton one side to GPIO5, the other to GND.
8. **Paste the Code:** Copy the entire contents of `src/main.cpp` and paste it into Wokwi's `sketch.ino` file.

## Part 3: Testing Correct Password Flow
9. **Start Simulation:** Click the green "Play" button. Wait 2 seconds for the OLED to initialize.
10. **Enter Password:** Click the keys `1`, `2`, `3`, `4` on the Wokwi keypad. Observe the `*` masking on the OLED.
11. **Verify Unlock:** The OLED should display "Unlocked!", the Servo arm should rotate to 90 degrees, and the Green LED should light up. Wait 5 seconds to verify the auto-lock feature (Servo returns to 0 degrees).

## Part 4: Testing Security Logic & Alarm
12. **First Wrong Attempt:** Enter `1`, `1`, `1`, `1`. The OLED will say "Wrong PIN!" and the Red LED will flash.
13. **Second Wrong Attempt:** Enter `2`, `2`, `2`, `2`. The system will warn you again.
14. **Trigger Lockout:** Enter `3`, `3`, `3`, `3`. The Red LED will stay ON, the buzzer will start pulsing (visible sound waves in Wokwi), and the OLED will say "LOCKED OUT". Try clicking keypad buttons—they will be ignored.

## Part 5: Diagnostics & Final Checks
15. **Open Serial Monitor:** Click on the Serial Monitor tab at the bottom of the Wokwi interface.
16. **View Telemetry:** Verify that every 2 seconds, the system logs its current state (`State: LOCKOUT`) without interrupting the buzzer's timing.
17. **Save & Capture:** Wait 30 seconds for the lockout to expire. Once the system resets to `LOCKED`, take screenshots of your circuit and terminal outputs as proof of execution for your project portfolio.
