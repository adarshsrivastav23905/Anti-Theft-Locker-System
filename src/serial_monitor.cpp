/**
 * =====================================================================================
 * @file    serial_monitor.cpp
 * @brief   UART Serial Telemetry Logger Implementation
 * @author  Adarsh Srivastav
 * 
 * @details Implements structured UART serial output for real-time monitoring.
 *          Two output modes:
 * 
 *          1. Event Logs (on-demand):
 *             Format:  [TAG] Message text
 *             Example: [AUTH] ACCESS GRANTED! Correct password.
 *             Tags: SYS, STATE, KEYPAD, AUTH, SECURITY, BUTTON, TIMED_OUT, ERROR
 * 
 *          2. Periodic Telemetry (every 2 seconds):
 *             Format:  [TELEMETRY] State: X | FailedAttempts: N | InputBuf: *** | ServoUnlocked: YES/NO
 *             Provides continuous system status for data logging and debugging.
 * 
 * @baud    115200 bps (8N1)
 * @refresh 2000ms telemetry interval
 * =====================================================================================
 */

#include "serial_monitor.h"

/**
 * @brief Constructor — initializes the telemetry timing variable.
 */
SerialMonitor::SerialMonitor() : lastTelemetryTime(0) {}

/**
 * @brief Initialize the UART serial interface and print boot banner.
 * The boot banner displays project title, architecture, subsystems, and author.
 * 
 * @param baudRate UART baud rate (default: 115200)
 */
void SerialMonitor::init(uint32_t baudRate) {
    Serial.begin(baudRate);
    delay(100);  // Allow UART hardware to stabilize
    Serial.println();
    Serial.println("==========================================================");
    Serial.println("  ANTI-THEFT LOCKER SYSTEM - EMBEDDED C++ PLATFORM");
    Serial.println("  Architecture: ESP32 DevKit V4 / Arduino Framework");
    Serial.println("  Subsystems: Keypad + OLED + Servo + Buzzer + EEPROM");
    Serial.println("  Author: Adarsh Srivastav");
    Serial.println("==========================================================");
    Serial.println("[SYS] System Initialized Successfully. Running FSM loop...");
}

/**
 * @brief Log a tagged event message to the serial monitor.
 * Used for one-time state transition events, errors, and user actions.
 * 
 * @param tag     Event category tag (e.g., "AUTH", "KEYPAD", "SECURITY")
 * @param message Human-readable event description
 */
void SerialMonitor::logEvent(const String &tag, const String &message) {
    Serial.print("[");
    Serial.print(tag);
    Serial.print("] ");
    Serial.println(message);
}

/**
 * @brief Broadcast periodic system telemetry (every 2 seconds).
 * Reports current FSM state, failed attempt count, masked input buffer,
 * and servo lock status. Useful for data logging and integration testing.
 * 
 * @param state          Current FSM state enum value
 * @param failedAttempts Number of consecutive failed password attempts
 * @param maskedPass     Asterisk-masked password buffer contents
 * @param isUnlocked     true if servo is at 90° (unlocked position)
 */
void SerialMonitor::logTelemetry(LockerState state, uint8_t failedAttempts, const String &maskedPass, bool isUnlocked) {
    if (millis() - lastTelemetryTime >= 2000) {
        lastTelemetryTime = millis();

        // Convert FSM state enum to human-readable string
        String stateStr = "UNKNOWN";
        switch (state) {
            case STATE_LOCKED: stateStr = "LOCKED"; break;
            case STATE_ENTERING_PASS: stateStr = "ENTERING_PASS"; break;
            case STATE_ACCESS_GRANTED: stateStr = "ACCESS_GRANTED"; break;
            case STATE_ACCESS_DENIED: stateStr = "ACCESS_DENIED"; break;
            case STATE_LOCKOUT: stateStr = "LOCKOUT"; break;
            case STATE_ALARM_ACTIVE: stateStr = "ALARM_ACTIVE"; break;
        }

        Serial.print("[TELEMETRY] State: ");
        Serial.print(stateStr);
        Serial.print(" | FailedAttempts: ");
        Serial.print(failedAttempts);
        Serial.print(" | InputBuf: ");
        Serial.print(maskedPass);
        Serial.print(" | ServoUnlocked: ");
        Serial.println(isUnlocked ? "YES" : "NO");
    }
}
