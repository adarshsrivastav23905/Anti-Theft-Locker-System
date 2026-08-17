/**
 * =====================================================================================
 * @file    lock_controller.h
 * @brief   SG90 Servo Motor Lock Actuator Controller
 * @author  Adarsh Srivastav
 * 
 * @details Controls the SG90 micro servo motor acting as the physical door latch
 *          mechanism. The servo operates on a 50Hz PWM signal:
 *          - Locked position:   0° (pulse width ≈ 1.0ms, duty cycle 5.0%)
 *          - Unlocked position: 90° (pulse width ≈ 1.5ms, duty cycle 7.5%)
 * 
 *          Tracks unlock timestamps for the auto-relock timer feature.
 *          The main loop checks elapsed time against AUTO_LOCK_DELAY_MS (5000ms)
 *          to automatically re-engage the lock after access is granted.
 * 
 * @pin     GPIO 18 (PWM Output)
 * @library ESP32Servo v3.0.5
 * =====================================================================================
 */

#ifndef LOCK_CONTROLLER_H
#define LOCK_CONTROLLER_H

#include <ESP32Servo.h>
#include "config.h"

class LockController {
private:
    Servo lockerServo;             // ESP32Servo instance for PWM generation
    bool isUnlockedState;          // Current lock state (true = open, false = locked)
    uint32_t unlockTimestamp;      // millis() timestamp when lock was last opened

public:
    LockController();
    void init();                   // Attach servo to GPIO and set initial locked position
    void unlockLocker();           // Rotate servo to 90° (unlocked) and record timestamp
    void lockLocker();             // Rotate servo to 0° (locked) and clear timestamp
    bool isUnlocked() const;      // Returns true if locker is currently unlocked
    uint32_t getUnlockTimestamp() const;  // Returns millis() when unlock occurred
};

#endif // LOCK_CONTROLLER_H
