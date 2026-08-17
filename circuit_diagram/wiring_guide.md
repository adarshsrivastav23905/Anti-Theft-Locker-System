# 🔌 Step-by-Step Breadboard Wiring Guide

## 1. Power Distribution Rails
- Connect ESP32 **5V / VIN** pin to the **Red (+) Power Rail** on the breadboard.
- Connect ESP32 **3.3V** pin to the **Auxiliary Red Power Rail** for OLED.
- Connect ESP32 **GND** pin to the **Blue (-) Ground Rail** on the breadboard.

## 2. SSD1306 OLED Display Connections
1. Connect OLED `VCC` to ESP32 `3.3V`.
2. Connect OLED `GND` to breadboard Ground Rail.
3. Connect OLED `SDA` to ESP32 `GPIO 21`.
4. Connect OLED `SCL` to ESP32 `GPIO 22`.

## 3. 4x4 Matrix Keypad Connections
1. Connect Keypad Row 1 pin to ESP32 `GPIO 13`.
2. Connect Keypad Row 2 pin to ESP32 `GPIO 12`.
3. Connect Keypad Row 3 pin to ESP32 `GPIO 14`.
4. Connect Keypad Row 4 pin to ESP32 `GPIO 27`.
5. Connect Keypad Col 1 pin to ESP32 `GPIO 26`.
6. Connect Keypad Col 2 pin to ESP32 `GPIO 25`.
7. Connect Keypad Col 3 pin to ESP32 `GPIO 33`.
8. Connect Keypad Col 4 pin to ESP32 `GPIO 32`.

## 4. Servo Motor Mechanical Lock Connections
1. Connect Servo `V+` (Red wire) to ESP32 `5V / VIN` power rail.
2. Connect Servo `GND` (Brown/Black wire) to Ground Rail.
3. Connect Servo `PWM Signal` (Orange/Yellow wire) to ESP32 `GPIO 18`.

## 5. Buzzer & LED Status Indicator Connections
1. **Red LED**: Connect Anode (+) to ESP32 `GPIO 2` via a `330Ω` resistor; Cathode (-) to Ground.
2. **Green LED**: Connect Anode (+) to ESP32 `GPIO 4` via a `330Ω` resistor; Cathode (-) to Ground.
3. **Active Buzzer**: Connect Positive (+) terminal to ESP32 `GPIO 15`; Negative (-) terminal to Ground.

## 6. Manual Lock Push Button Connection
1. Connect Switch Terminal 1 to ESP32 `GPIO 5`.
2. Connect Switch Terminal 2 to Ground Rail.
