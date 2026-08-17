# Circuit Diagram & Architecture

## System Architecture Overview

```text
                                +-------------------+
                                |    POWER SUPPLY   |
                                |      5V USB       |
                                +-------------------+
                                         |
                                         v
+----------------+              +-------------------+              +-------------------+
|     INPUTS     |              |     CONTROL       |              |     OUTPUTS     |
|                |              |                   |              |                   |
|  [4x4 Keypad]  |======(8)====>|   ESP32 DevKit    |====(I2C)====>|   [SSD1306 OLED]  |
|                |              |                   |              |                   |
|  [Push Button] |------(1)---->|    (MCU Core)     |----(PWM)---->|   [SG90 Servo]    |
|                |              |                   |              |                   |
+----------------+              |      (NVS)        |----(Dig)---->|   [Active Buzzer] |
                                |                   |              |                   |
                                |     (Timers)      |----(Dig)---->|   [Red LED]       |
                                |                   |              |                   |
                                +-------------------+----(Dig)---->|   [Green LED]     |
                                         |                         +-------------------+
                                       (UART)
                                         |
                                         v
                                +-------------------+
                                |   SERIAL MONITOR  |
                                +-------------------+
```

## I/O Mapping Table

| Component | ESP32 Pin | Domain | Direction | Protocol / Logic |
| :--- | :--- | :--- | :--- | :--- |
| **Keypad Row 1** | GPIO13 | Digital I/O | Output | Matrix Scanning (LOW active) |
| **Keypad Row 2** | GPIO12 | Digital I/O | Output | Matrix Scanning (LOW active) |
| **Keypad Row 3** | GPIO14 | Digital I/O | Output | Matrix Scanning (LOW active) |
| **Keypad Row 4** | GPIO27 | Digital I/O | Output | Matrix Scanning (LOW active) |
| **Keypad Col 1** | GPIO26 | Digital I/O | Input | Internal Pull-Up, Active LOW |
| **Keypad Col 2** | GPIO25 | Digital I/O | Input | Internal Pull-Up, Active LOW |
| **Keypad Col 3** | GPIO33 | Digital I/O | Input | Internal Pull-Up, Active LOW |
| **Keypad Col 4** | GPIO32 | Digital I/O | Input | Internal Pull-Up, Active LOW |
| **OLED SDA** | GPIO21 | I2C Bus | In/Out | I2C Data (0x3C) |
| **OLED SCL** | GPIO22 | I2C Bus | Output | I2C Clock |
| **Servo Motor** | GPIO18 | PWM | Output | 50Hz, 1-2ms Pulse Width |
| **Active Buzzer** | GPIO15 | Digital | Output | 3.3V Logic HIGH = Sound |
| **Red LED** | GPIO2 | Digital | Output | 3.3V Logic HIGH = ON |
| **Green LED** | GPIO4 | Digital | Output | 3.3V Logic HIGH = ON |
| **Push Button** | GPIO5 | Digital | Input | Internal Pull-Up, Active LOW |
| **UART TX/RX** | TX0/RX0 | Serial | In/Out | 115200 Baud (Hardware USB) |

## Signal Timing Characteristics

| Signal Type | Component | Timing Parameter | Value |
| :--- | :--- | :--- | :--- |
| **Clock** | I2C (SCL) | Bus Speed | 100 kHz (Standard Mode) |
| **Frequency** | SG90 Servo | PWM Frequency | 50 Hz (20ms Period) |
| **Pulse Width**| SG90 Servo | Duty Cycle (0°) | ~1.0 ms (Locked) |
| **Pulse Width**| SG90 Servo | Duty Cycle (90°) | ~2.0 ms (Unlocked) |
| **Interval** | Active Buzzer | Siren Pulsation | 300 ms ON / 300 ms OFF |
| **Debounce** | Keypad | Read Delay | 50 ms (Software handled) |
| **Timeout** | FSM | Auto-Lock Delay | 5,000 ms (5 Seconds) |
| **Timeout** | FSM | Lockout Penalty | 30,000 ms (30 Seconds) |
| **Polling** | UART Telemetry| Log Frequency | 2,000 ms (2 Seconds) |

## Power Distribution Diagram

```text
[ USB Power Source (5V, 1A) ]
       |
       |--- (VBUS) ---> [ ESP32 Voltage Regulator ] ---> [ 3.3V Rail ]
       |                                                    |
       |--- (5V Rail)                                       |---> ESP32 Logic Core
               |                                            |
               |---> SG90 Servo (VCC)                       |---> OLED Display (VCC)
               |                                            |
               |---> Active Buzzer (VCC)*                   |---> Keypad Pull-ups (Internal)
                                                            |
                                                            |---> Red/Green LEDs (Anodes via 330Ω)
                                                            |
                                                            |---> Push Button (via internal pull-up)

*Note: In physical builds, a 5V buzzer triggers off 3.3V logic using an NPN transistor, or use a 3.3V rated buzzer directly on the rail.
```
