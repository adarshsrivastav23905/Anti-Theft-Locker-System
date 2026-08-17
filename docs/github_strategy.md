# GitHub Strategy & Portfolio Plan

A well-structured GitHub repository acts as a live portfolio for technical interviews.

## Repository Details
- **Best Repo Name:** `ESP32-AntiTheft-Locker` or `Smart-Locker-FSM-ESP32`
- **Description:** "An ESP32-based Anti-Theft Locker System utilizing an FSM architecture, I2C OLED masking, matrix keypad scanning, and brute-force lockout logic. Built with PlatformIO."
- **10 GitHub Tags:** `esp32`, `cpp`, `embedded-systems`, `platformio`, `wokwi`, `finite-state-machine`, `iot-security`, `arduino-framework`, `ssd1306`, `hardware-simulation`

## 8-Day Commit Strategy

| Day | Focus Area | Files to Commit | Commit Message | Proof / Visual |
| :--- | :--- | :--- | :--- | :--- |
| **Day 1** | Project Init | `platformio.ini`, `src/main.cpp` (empty) | `init: PlatformIO setup and dependencies` | Terminal screenshot of build success |
| **Day 2** | Hardware UI | `src/main.cpp` (I2C/OLED/Keypad logic) | `feat: integrate SSD1306 OLED and 4x4 Keypad` | Photo/Wokwi screenshot of OLED text |
| **Day 3** | Core Auth | `src/main.cpp` (Password logic) | `feat: implement PIN masking and auth logic` | GIF of entering a PIN (`****`) |
| **Day 4** | Actuators | `src/main.cpp` (Servo, LEDs) | `feat: add SG90 servo and LED state indicators` | Wokwi screenshot of Servo at 90° |
| **Day 5** | FSM & Security | `src/main.cpp` (FSM, Alarm, Auto-lock) | `feat: refactor to FSM, add lockout & auto-lock` | GIF of alarm triggering after 3 fails |
| **Day 6** | NVS Storage | `src/main.cpp` (Preferences API) | `feat: implement NVS for password persistence` | Serial monitor log showing NVS read |
| **Day 7** | Documentation | `docs/*`, `README.md` | `docs: add extensive technical documentation` | Repo layout screenshot |
| **Day 8** | Simulation | `simulation/*`, `diagram.json` | `test: add Wokwi simulation config and test reports`| Link to live Wokwi project in README |

## Exact Git Commands

To execute this strategy locally and push to GitHub:

```bash
# Day 1 Example
git init
git add platformio.ini src/main.cpp
git commit -m "init: PlatformIO setup and dependencies"
git branch -M main
git remote add origin https://github.com/YourUsername/ESP32-AntiTheft-Locker.git
git push -u origin main

# Daily workflow thereafter
git status
git add .
git commit -m "feat: [insert feature here]"
git push
```
