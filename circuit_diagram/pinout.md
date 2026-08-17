# 📍 Hardware Pin Mapping Table

| Component Name | Pin Function | ESP32 DevKit V4 Pin | Electrical Domain | Signal Description |
|---|---|---:|---|---|
| **4x4 Keypad** | Row 1 (R1) | GPIO 13 | Digital Input (Pull-up) | Keypad Row 1 Scanning |
| | Row 2 (R2) | GPIO 12 | Digital Input (Pull-up) | Keypad Row 2 Scanning |
| | Row 3 (R3) | GPIO 14 | Digital Input (Pull-up) | Keypad Row 3 Scanning |
| | Row 4 (R4) | GPIO 27 | Digital Input (Pull-up) | Keypad Row 4 Scanning |
| | Column 1 (C1) | GPIO 26 | Digital Output | Keypad Column 1 Drive |
| | Column 2 (C2) | GPIO 25 | Digital Output | Keypad Column 2 Drive |
| | Column 3 (C3) | GPIO 33 | Digital Output | Keypad Column 3 Drive |
| | Column 4 (C4) | GPIO 32 | Digital Output | Keypad Column 4 Drive |
| **SSD1306 OLED** | VCC | 3.3V | Power Rail | +3.3V Power Supply |
| | GND | GND | Power Rail | Ground Reference |
| | SDA | GPIO 21 | I2C Data | Serial Data Line |
| | SCL | GPIO 22 | I2C Clock | Serial Clock Line |
| **Servo Motor** | Signal (PWM) | GPIO 18 | PWM Output | Servo Position Control |
| | VCC (V+) | 5V / VIN | Power Rail | +5V DC Supply Rail |
| | GND | GND | Power Rail | Common Ground |
| **Active Buzzer** | Positive (+) | GPIO 15 | Digital Output | High-frequency Security Siren |
| | Negative (-) | GND | Power Rail | Ground Reference |
| **Red LED** | Anode (+) | GPIO 2 | Digital Output (330Ω) | Access Denied / Alarm Active |
| | Cathode (-) | GND | Power Rail | Ground Reference |
| **Green LED** | Anode (+) | GPIO 4 | Digital Output (330Ω) | Access Granted / Unlocked |
| | Cathode (-) | GND | Power Rail | Ground Reference |
| **Lock Switch** | Terminal 1 | GPIO 5 | Digital Input (Pull-up) | Manual Lock / System Reset |
| | Terminal 2 | GND | Power Rail | Ground Reference |
