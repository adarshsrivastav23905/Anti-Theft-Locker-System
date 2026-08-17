# System Limitations & Future Roadmap

Every engineered system involves trade-offs. Below are the 9 known limitations of the current Anti-Theft Locker System and the roadmap for addressing them in future iterations.

## Known Limitations

1. **No Network Connectivity:** Despite using an ESP32 (which has native Wi-Fi/Bluetooth), the system currently operates purely offline. There are no remote alerts if the vault is tampered with.
2. **Volatile Alarm State Memory:** If an intruder triggers the 30-second lockout and immediately unplugs/replugs the power, the system reboots into `STATE_LOCKED` with zero failed attempts, bypassing the penalty.
3. **Software Debounce Only:** The system relies entirely on the `Keypad.h` software debounce. High electrical noise environments could theoretically cause ghost button presses.
4. **Simulated Physical Loads:** The SG90 servo motor simulates the locking bar. In reality, an SG90 lacks the torque to secure a heavy metal door.
5. **Fixed Password Length:** The system strictly expects a 4-digit PIN. Users cannot opt for a stronger 6 or 8-digit password.
6. **Single User Only:** There is only one master PIN stored in NVS. Multi-tenant environments (like gym lockers) cannot support different PINs for different users.
7. **Fixed Thresholds:** The auto-lock (5s) and lockout penalty (30s) are hardcoded. A user cannot customize these settings via the UI.
8. **Display Density:** The 128x64 OLED is small. While excellent for basic prompts, it is difficult to read from a distance or for visually impaired users.
9. **Virtual Prototype Constraints:** The current codebase is heavily optimized and tested for the Wokwi simulator. Physical deployment requires careful consideration of ESP32 power regulation when driving motors.

---

## Future Improvement Roadmap

| Limitation Addressed | Proposed Upgrade | Implementation Strategy |
| :--- | :--- | :--- |
| **#1 (No Network)** | **IoT Integration** | Connect to WiFi, use MQTT or Telegram Bot API to send a push notification when `STATE_LOCKOUT` triggers. |
| **#2 (Volatile State)**| **NVS State Logging** | Save the `failedAttempts` counter and a lockout timestamp to NVS immediately upon failure, checking it in `setup()` on boot. |
| **#4 (Weak Motor)** | **Solenoid Actuator** | Replace the SG90 servo with a 12V Solenoid lock, driven by a MOSFET (e.g., IRLZ44N) isolated from the ESP32 logic level. |
| **#5 & #6 (Auth)** | **Dynamic Array & RFID** | Implement an RFID reader (RC522 via SPI). Allow multi-user access (Card OR Pin), and dynamic array parsing for variable length PINs ending with `#`. |
| **#7 (Fixed Config)** | **Settings Menu** | Utilize keys `A`, `B`, `C`, `D` on the keypad to navigate an OLED settings menu to change PIN, timeout limits, and buzzer volume. |
