# Tech Stack Options

When designing the Anti-Theft Locker System, three distinct technical architectures were evaluated. 

## Comparison Table

| Feature / Metric | Option A (Easy) | Option B (Recommended) | Option C (Advanced) |
| :--- | :--- | :--- | :--- |
| **Components** | Arduino UNO, 16x2 LCD, Keypad, DC Motor | ESP32, 128x64 OLED, Keypad, SG90 Servo | ESP32, TFT Touch, RFID, Solenoid, WiFi/App |
| **Difficulty** | Beginner | Intermediate | Advanced |
| **Output / Display**| Basic text, heavy wiring (Parallel LCD) | High-res text/graphics, 2-wire I2C | Full graphical UI, network connectivity |
| **Hardware Mandatory?**| Yes | Yes (Simulatable in Wokwi) | Yes (Hard to simulate IoT fully) |
| **Storage** | EEPROM (Limited cycles) | NVS Preferences (Robust flash) | NVS + Cloud Database |
| **Best For** | High school projects, introduction to C++ | Diploma/Degree portfolios, robust FSM logic | Production-level prototypes, IoT showcases |

## Selected Architecture: Option B (ESP32 + OLED + Keypad)

**Why Option B was selected:**
Option B strikes the perfect balance between complexity, feature richness, and academic rigor for an Embedded Systems final project.

1.  **Processing Power & RTOS Potential:** Unlike the 8-bit, 16MHz Arduino UNO, the ESP32 is a 32-bit, 240MHz powerhouse. While this project runs on a bare-metal loop with `millis()`, the ESP32 leaves immense headroom for future FreeRTOS integration.
2.  **I2C over Parallel:** Using an I2C OLED (SSD1306) requires only 2 pins (SDA/SCL) compared to the 6+ pins required for a standard 16x2 character LCD. This frees up crucial GPIOs for the keypad matrix and sensors.
3.  **Modern Storage (NVS):** The Arduino relies on traditional EEPROM, which is archaic and strict on wear leveling. The ESP32's Non-Volatile Storage (NVS) API manages flash sectors abstractly, providing a modern key-value pair approach to saving the password.
4.  **Simulation Viability:** The exact stack (ESP32 + OLED + Servo) is 100% supported by the Wokwi simulator, allowing rapid software iteration, debugging, and verification without dealing with loose physical wires during the development phase.
