/**
 * =====================================================================================
 * @file    security_manager.h
 * @brief   Authentication Engine, FSM Controller & NVS Persistence Manager
 * @author  Adarsh Srivastav
 * 
 * @details Core security subsystem managing password verification, failed attempt
 *          tracking, FSM state transitions, and non-volatile flash storage persistence.
 * 
 *          Authentication Flow:
 *          1. User enters PIN via keypad → raw string passed to verifyPassword()
 *          2. String compared against NVS-stored active password
 *          3. Match → STATE_ACCESS_GRANTED, counter reset
 *          4. Mismatch → failedAttempts++, persisted to NVS
 *          5. failedAttempts >= 3 → STATE_LOCKOUT → STATE_ALARM_ACTIVE
 * 
 *          NVS Storage Keys (namespace: "locker-sec"):
 *          - "pass":        String — Current active PIN code
 *          - "failedCount": uint8_t — Consecutive failed attempt counter
 * 
 * @library Preferences (ESP32 Arduino Core built-in)
 * =====================================================================================
 */

#ifndef SECURITY_MANAGER_H
#define SECURITY_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include "config.h"

class SecurityManager {
private:
    String activePassword;             // Currently active PIN (loaded from NVS)
    uint8_t failedAttempts;            // Consecutive wrong attempt counter
    LockerState currentState;          // Current FSM state
    uint32_t lockoutStartTimestamp;    // millis() when lockout was triggered
    Preferences preferences;          // ESP32 NVS flash storage handle

public:
    SecurityManager();
    void init();                       // Open NVS namespace, load password & attempt count
    bool verifyPassword(const String &inputPass);  // Compare input against stored password
    void registerFailedAttempt();       // Increment and persist failed attempt counter
    void resetFailedAttempts();         // Reset counter to 0 and persist
    uint8_t getFailedAttempts() const;  // Returns current consecutive failure count
    uint8_t getRemainingAttempts() const;  // Returns (MAX - current) attempts
    LockerState getState() const;       // Returns current FSM state
    void setState(LockerState state);   // Force FSM state transition
    bool isLockoutActive() const;       // Check if lockout timer is still running
    uint32_t getRemainingLockoutSeconds() const;  // Seconds remaining in lockout
    bool changePassword(const String &newPass);    // Update password (must match length)
    String getActivePassword() const;   // Returns current password (debug/admin only)
};

#endif // SECURITY_MANAGER_H
