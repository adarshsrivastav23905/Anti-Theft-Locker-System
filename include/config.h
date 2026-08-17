/**
 * =====================================================================================
 * @file    config.h
 * @brief   Central Configuration & Pin Definitions for Anti-Theft Locker System
 * @author  Adarsh Srivastav
 * @date    2026
 * 
 * @details This header defines all hardware pin assignments, system timing parameters,
 *          security thresholds, and FSM state enumeration for the Anti-Theft Locker
 *          System. All configurable constants are centralized here to enable easy
 *          modification without touching module source files.
 * 
 * @target  ESP32 DevKit V4 (Xtensa LX6 Dual-Core 240MHz)
 * @framework Arduino / PlatformIO
 * =====================================================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==========================================
// OLED DISPLAY CONFIGURATION (I2C Bus)
// ==========================================
// SSD1306 128x64 OLED display connected via I2C protocol
// I2C Clock: 400kHz (Fast Mode)
// I2C Address: 0x3C (default for most SSD1306 modules)

#define OLED_SDA_PIN        21      // I2C Serial Data Line
#define OLED_SCL_PIN        22      // I2C Serial Clock Line
#define SCREEN_WIDTH        128     // OLED display width in pixels
#define SCREEN_HEIGHT       64      // OLED display height in pixels
#define OLED_RESET          -1      // No hardware reset pin (shared with ESP32 EN)
#define OLED_I2C_ADDRESS    0x3C    // Default I2C slave address for SSD1306

// ==========================================
// 4x4 MATRIX KEYPAD PIN ASSIGNMENTS
// ==========================================
// Matrix keypad uses drive-and-sense row-column scanning.
// Rows: Configured as INPUT with internal pull-up resistors
// Columns: Configured as OUTPUT, driven LOW sequentially
// Total: 8 GPIO pins to scan 16 keys (4 rows × 4 columns)

#define KEYPAD_ROW_1        13      // Row 1 — Keys: 1, 2, 3, A
#define KEYPAD_ROW_2        12      // Row 2 — Keys: 4, 5, 6, B
#define KEYPAD_ROW_3        14      // Row 3 — Keys: 7, 8, 9, C
#define KEYPAD_ROW_4        27      // Row 4 — Keys: *, 0, #, D

#define KEYPAD_COL_1        26      // Column 1 — Keys: 1, 4, 7, *
#define KEYPAD_COL_2        25      // Column 2 — Keys: 2, 5, 8, 0
#define KEYPAD_COL_3        33      // Column 3 — Keys: 3, 6, 9, #
#define KEYPAD_COL_4        32      // Column 4 — Keys: A, B, C, D

// ==========================================
// ACTUATORS & INDICATOR PIN ASSIGNMENTS
// ==========================================
// Servo: 50Hz PWM signal (1.0ms = 0°, 1.5ms = 90°, 2.0ms = 180°)
// Buzzer: Active buzzer (HIGH = sound, LOW = silent)
// LEDs: Current limited via external 330Ω resistors (~10mA)

#define SERVO_PIN           18      // SG90 Servo motor PWM signal pin
#define BUZZER_PIN          15      // Active 5V buzzer positive terminal
#define RED_LED_PIN         2       // Red LED anode (Access Denied / Alarm)
#define GREEN_LED_PIN       4       // Green LED anode (Access Granted / Unlocked)
#define LOCK_BUTTON_PIN     5       // Manual lock / system reset push button

// ==========================================
// SECURITY & AUTHENTICATION PARAMETERS
// ==========================================
// These parameters define the security behavior of the locker system.
// Modify these values to adjust sensitivity and timing.

#define DEFAULT_PASSWORD       "1234"   // Factory default 4-digit PIN code
#define PASSWORD_LENGTH        4        // Fixed password length (digits)
#define MAX_FAILED_ATTEMPTS    3        // Consecutive wrong attempts before lockout

// ==========================================
// SYSTEM TIMING CONSTANTS (milliseconds)
// ==========================================
// All timing uses non-blocking millis() cooperative scheduling.
// No blocking delay() calls are used in the main event loop.

#define AUTO_LOCK_DELAY_MS     5000    // Auto-relock timeout after access granted
#define LOCKOUT_DURATION_MS    30000   // Security lockout penalty duration
#define BUZZER_PULSE_INTERVAL  300     // Siren ON/OFF toggle period (f ≈ 1.66 Hz)
#define DEBOUNCE_DELAY_MS      50      // Software switch debounce threshold

// ==========================================
// FINITE STATE MACHINE (FSM) DEFINITIONS
// ==========================================
// The locker system operates as a deterministic FSM with 6 states.
// State transitions are driven by keypad input, timers, and security logic.

enum LockerState {
    STATE_LOCKED,           // Default armed state — servo locked, awaiting PIN entry
    STATE_ENTERING_PASS,    // User is actively typing digits on the keypad
    STATE_ACCESS_GRANTED,   // Correct PIN verified — servo unlocked, auto-lock timer running
    STATE_ACCESS_DENIED,    // Incorrect PIN entered — brief error display (2 seconds)
    STATE_LOCKOUT,          // Max failed attempts reached — transitioning to alarm
    STATE_ALARM_ACTIVE      // Security breach — pulsing siren, red LED, 30s lockout
};

#endif // CONFIG_H
