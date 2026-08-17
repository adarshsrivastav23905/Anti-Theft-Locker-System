/**
 * =====================================================================================
 * @file    lock_controller.cpp
 * @brief   SG90 Servo Motor Lock Actuator Implementation
 * @author  Adarsh Srivastav
 * 
 * @details Controls the servo motor acting as the mechanical door latch.
 *          
 *          PWM Signal Specifications:
 *          - Frequency: 50 Hz (period = 20ms)
 *          - Locked (0°):   pulse width ≈ 1.0ms → duty cycle = 5.0%
 *          - Unlocked (90°): pulse width ≈ 1.5ms → duty cycle = 7.5%
 *          
 *          The ESP32Servo library handles hardware timer allocation and
 *          LEDC channel configuration automatically.
 * =====================================================================================
 */

#include "lock_controller.h"

/**
 * @brief Constructor — initializes state tracking variables.
 */
LockController::LockController() : isUnlockedState(false), unlockTimestamp(0) {}

/**
 * @brief Attach servo to GPIO pin and set initial locked position.
 * The servo starts at 0° (locked) on every system boot.
 */
void LockController::init() {
    lockerServo.attach(SERVO_PIN);
    lockLocker(); // Start in locked state by default
}

/**
 * @brief Rotate servo to 90° (unlocked position).
 * Records the current millis() timestamp for the auto-lock timer.
 * The main loop checks this timestamp against AUTO_LOCK_DELAY_MS.
 */
void LockController::unlockLocker() {
    lockerServo.write(90); // 90° = Unlocked position (latch disengaged)
    isUnlockedState = true;
    unlockTimestamp = millis();
}

/**
 * @brief Rotate servo to 0° (locked position).
 * Clears the unlock timestamp to disable the auto-lock timer.
 */
void LockController::lockLocker() {
    lockerServo.write(0); // 0° = Locked position (latch engaged)
    isUnlockedState = false;
    unlockTimestamp = 0;
}

/**
 * @brief Check if the locker is currently in the unlocked state.
 * @return true if servo is at 90° (unlocked), false if at 0° (locked)
 */
bool LockController::isUnlocked() const {
    return isUnlockedState;
}

/**
 * @brief Get the millis() timestamp when the locker was last unlocked.
 * Used by the main loop to calculate elapsed time for auto-lock.
 * @return Timestamp in milliseconds, or 0 if locked.
 */
uint32_t LockController::getUnlockTimestamp() const {
    return unlockTimestamp;
}
