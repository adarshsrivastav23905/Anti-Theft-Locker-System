/**
 * =====================================================================================
 * @file    security_manager.cpp
 * @brief   Authentication Engine, FSM Controller & NVS Flash Persistence
 * @author  Adarsh Srivastav
 * 
 * @details Core security subsystem that handles:
 *          1. Password verification (string comparison)
 *          2. Failed attempt counting and persistence in NVS flash
 *          3. FSM state transition management
 *          4. Security lockout timing (30-second penalty)
 *          5. Password change functionality
 * 
 *          NVS (Non-Volatile Storage) is used instead of EEPROM because ESP32
 *          does not have true EEPROM — the Arduino EEPROM library on ESP32 is
 *          actually a wrapper around NVS/Flash anyway. The Preferences library
 *          provides a cleaner, key-value API directly.
 * 
 *          Namespace: "locker-sec"
 *          Keys: "pass" (String), "failedCount" (uint8_t)
 * =====================================================================================
 */

#include "security_manager.h"

/**
 * @brief Constructor — sets default password and initial state.
 * NVS is not opened here because it requires Arduino runtime to be initialized.
 */
SecurityManager::SecurityManager() 
    : activePassword(DEFAULT_PASSWORD), failedAttempts(0), currentState(STATE_LOCKED), lockoutStartTimestamp(0) {}

/**
 * @brief Initialize NVS flash storage and load persistent data.
 * Opens the "locker-sec" namespace in read-write mode.
 * Loads the stored password (or uses DEFAULT_PASSWORD if first boot).
 * Loads the failed attempt counter (persists across power cycles).
 */
void SecurityManager::init() {
    preferences.begin("locker-sec", false);  // false = read-write mode
    activePassword = preferences.getString("pass", DEFAULT_PASSWORD);
    failedAttempts = preferences.getUChar("failedCount", 0);
    currentState = STATE_LOCKED;
}

/**
 * @brief Verify an entered password against the stored active password.
 * 
 * On match:  Resets failed counter, transitions to STATE_ACCESS_GRANTED.
 * On mismatch: Increments failed counter, persists to NVS.
 *              If count >= MAX_FAILED_ATTEMPTS: transitions to STATE_LOCKOUT.
 *              Otherwise: transitions to STATE_ACCESS_DENIED.
 * 
 * @param inputPass The raw password string entered by the user
 * @return true if password matches, false otherwise
 */
bool SecurityManager::verifyPassword(const String &inputPass) {
    if (inputPass == activePassword) {
        resetFailedAttempts();
        currentState = STATE_ACCESS_GRANTED;
        return true;
    } else {
        registerFailedAttempt();
        if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
            currentState = STATE_LOCKOUT;
            lockoutStartTimestamp = millis();  // Start 30-second lockout timer
        } else {
            currentState = STATE_ACCESS_DENIED;
        }
        return false;
    }
}

/**
 * @brief Increment the failed attempt counter and persist to NVS.
 * This ensures the counter survives power cycles — a thief cannot simply
 * power-cycle the system to reset the attempt count.
 */
void SecurityManager::registerFailedAttempt() {
    failedAttempts++;
    preferences.putUChar("failedCount", failedAttempts);
}

/**
 * @brief Reset the failed attempt counter to zero and persist to NVS.
 * Called after a successful authentication or manual system reset.
 */
void SecurityManager::resetFailedAttempts() {
    failedAttempts = 0;
    preferences.putUChar("failedCount", 0);
}

/**
 * @brief Returns the current consecutive failed attempt count.
 */
uint8_t SecurityManager::getFailedAttempts() const {
    return failedAttempts;
}

/**
 * @brief Calculate remaining attempts before lockout triggers.
 * @return (MAX_FAILED_ATTEMPTS - failedAttempts), minimum 0
 */
uint8_t SecurityManager::getRemainingAttempts() const {
    if (failedAttempts >= MAX_FAILED_ATTEMPTS) return 0;
    return MAX_FAILED_ATTEMPTS - failedAttempts;
}

/**
 * @brief Returns the current FSM state.
 */
LockerState SecurityManager::getState() const {
    return currentState;
}

/**
 * @brief Force a FSM state transition.
 * Used by the main loop for timer-driven transitions (auto-lock, lockout expiry).
 */
void SecurityManager::setState(LockerState state) {
    currentState = state;
}

/**
 * @brief Check if the security lockout timer is still active.
 * Compares elapsed time since lockout trigger against LOCKOUT_DURATION_MS (30s).
 * @return true if the 30-second penalty is still running
 */
bool SecurityManager::isLockoutActive() const {
    if (currentState == STATE_LOCKOUT || currentState == STATE_ALARM_ACTIVE) {
        if (millis() - lockoutStartTimestamp < LOCKOUT_DURATION_MS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Calculate remaining seconds in the lockout period.
 * Used to display the countdown timer on the OLED lockout screen.
 * @return Remaining seconds (0 if lockout has expired or is not active)
 */
uint32_t SecurityManager::getRemainingLockoutSeconds() const {
    if (!isLockoutActive()) return 0;
    uint32_t elapsed = millis() - lockoutStartTimestamp;
    if (elapsed >= LOCKOUT_DURATION_MS) return 0;
    return (LOCKOUT_DURATION_MS - elapsed) / 1000;
}

/**
 * @brief Change the active password and persist to NVS flash.
 * Only accepts passwords that match the configured PASSWORD_LENGTH.
 * 
 * @param newPass The new password string (must be exactly PASSWORD_LENGTH chars)
 * @return true if password was changed successfully, false if length mismatch
 */
bool SecurityManager::changePassword(const String &newPass) {
    if (newPass.length() == PASSWORD_LENGTH) {
        activePassword = newPass;
        preferences.putString("pass", newPass);
        return true;
    }
    return false;
}

/**
 * @brief Returns the current active password (for debug/admin purposes only).
 * WARNING: This should never be displayed on the OLED screen in production.
 */
String SecurityManager::getActivePassword() const {
    return activePassword;
}
