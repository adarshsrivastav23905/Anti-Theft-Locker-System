/**
 * =====================================================================================
 * @file    main.cpp
 * @brief   Main Application Orchestrator — Anti-Theft Locker System
 * @author  Adarsh Srivastav
 * 
 * @details This is the primary entry point for the Anti-Theft Locker System firmware.
 *          It initializes all hardware subsystems and runs the main non-blocking
 *          event loop that:
 *          
 *          1. Checks the manual lock/reset push button (GPIO 5) with debouncing
 *          2. Polls the 4x4 matrix keypad for user input
 *          3. Processes digit entry, clear (*), and submit (#) commands
 *          4. Delegates authentication to the SecurityManager FSM
 *          5. Controls the servo motor lock via LockController
 *          6. Updates the SSD1306 OLED display for each state
 *          7. Manages alarm siren pulsing via AlarmManager
 *          8. Handles auto-lock timeout (5s) and lockout expiration (30s)
 *          9. Broadcasts UART telemetry every 2 seconds
 * 
 *          The main loop is entirely non-blocking — all timing uses millis()
 *          cooperative scheduling to maintain responsiveness.
 * 
 * @target  ESP32 DevKit V4 (Xtensa LX6 Dual-Core 240MHz)
 * @framework Arduino / PlatformIO
 * =====================================================================================
 */

#include <Arduino.h>
#include "config.h"
#include "keypad_manager.h"
#include "display_manager.h"
#include "lock_controller.h"
#include "alarm_manager.h"
#include "security_manager.h"
#include "serial_monitor.h"

// =====================================================================================
// GLOBAL SUBSYSTEM MANAGER INSTANCES
// =====================================================================================
// Each manager encapsulates a hardware peripheral and its associated logic.
// Instantiated globally to persist state across loop() iterations.

KeypadManager keypadMgr;       // 4x4 matrix keypad scanner and input buffer
DisplayManager displayMgr;    // SSD1306 OLED I2C display renderer
LockController lockCtrl;      // SG90 servo motor lock actuator
AlarmManager alarmMgr;        // Buzzer siren and LED status indicators
SecurityManager secMgr;       // Authentication engine, FSM controller, NVS storage
SerialMonitor logger;         // UART telemetry logger (115200 baud)

// Timing variables for non-blocking state transitions
uint32_t stateTransitionTimer = 0;   // General-purpose state display timer
uint32_t lastButtonCheckTime = 0;    // Debounce timer for manual lock button

/**
 * @brief Hardware initialization and subsystem boot sequence.
 * 
 * Initialization order matters:
 * 1. Serial (UART) — for early debug output
 * 2. GPIO pins — manual button with internal pull-up
 * 3. Alarm (buzzer + LEDs) — set outputs LOW
 * 4. Lock (servo) — attach PWM and set locked position (0°)
 * 5. Keypad — initialize matrix scanner
 * 6. Security — open NVS flash namespace, load password & attempt count
 * 7. Display (OLED) — initialize I2C bus and SSD1306 controller
 * 8. Splash screen — show boot animation for 2 seconds
 */
void setup() {
    logger.init(115200);

    pinMode(LOCK_BUTTON_PIN, INPUT_PULLUP);

    alarmMgr.init();
    lockCtrl.init();
    keypadMgr.init();
    secMgr.init();

    if (!displayMgr.init()) {
        logger.logEvent("ERROR", "SSD1306 OLED initialization failed!");
    }

    displayMgr.showSplashScreen();
    delay(2000);
    
    displayMgr.showLockedScreen("");
    logger.logEvent("STATE", "System armed in STATE_LOCKED");
}

/**
 * @brief Main non-blocking event loop.
 * 
 * Execution flow per iteration:
 * 1. Read current FSM state
 * 2. Check manual lock button (with 50ms debounce)
 * 3. Poll keypad for key press
 * 4. Process key: digit (buffer), '*' (clear), '#' (submit/authenticate)
 * 5. Handle FSM state-specific timers (auto-lock, denied display, alarm pulse, lockout)
 * 6. Broadcast periodic UART telemetry
 */
void loop() {
    LockerState state = secMgr.getState();

    // -------------------------------------------------------------
    // PRIORITY 1: Manual Lock / System Reset Push Button (GPIO 5)
    // Active LOW with INPUT_PULLUP — pressing connects GPIO to GND.
    // Immediately overrides all states: locks servo, stops alarm,
    // resets failure counter, returns to STATE_LOCKED.
    // -------------------------------------------------------------
    if (digitalRead(LOCK_BUTTON_PIN) == LOW) {
        if (millis() - lastButtonCheckTime > DEBOUNCE_DELAY_MS) {
            lastButtonCheckTime = millis();
            logger.logEvent("BUTTON", "Manual Lock / Reset Button Pressed!");
            
            lockCtrl.lockLocker();
            alarmMgr.stopAlarm();
            secMgr.resetFailedAttempts();
            secMgr.setState(STATE_LOCKED);
            keypadMgr.resetBuffer();
            displayMgr.showLockedScreen("");
            delay(300);
            return;
        }
    }

    // -------------------------------------------------------------
    // PRIORITY 2: Keypad Input Processing
    // Reads one key per loop iteration (non-blocking).
    // Beeps on every valid keypress for tactile feedback.
    // -------------------------------------------------------------
    char key = keypadMgr.getKey();
    if (key) {
        alarmMgr.beepShort();
        
        String inputPass;
        bool isSubmitted = false;
        bool isCleared = false;

        keypadMgr.processInput(key, inputPass, isSubmitted, isCleared);

        if (isCleared) {
            // '*' key pressed — clear input buffer and return to locked screen
            logger.logEvent("KEYPAD", "Input Cleared (*)");
            if (secMgr.getState() == STATE_LOCKED || secMgr.getState() == STATE_ENTERING_PASS) {
                secMgr.setState(STATE_LOCKED);
                displayMgr.showLockedScreen("");
            }
        } 
        else if (isSubmitted) {
            // '#' key pressed — submit entered password for authentication
            logger.logEvent("KEYPAD", "Submitted Password (#)");
            String entered = keypadMgr.getRawPassword();
            keypadMgr.resetBuffer();

            if (secMgr.verifyPassword(entered)) {
                // ✓ Correct password — unlock servo, green LED, double beep
                logger.logEvent("AUTH", "ACCESS GRANTED! Correct password.");
                lockCtrl.unlockLocker();
                alarmMgr.setGrantedState();
                displayMgr.showAccessGranted();
                stateTransitionTimer = millis();
            } else {
                // ✗ Wrong password — red LED flash, increment counter
                logger.logEvent("AUTH", "ACCESS DENIED! Incorrect password.");
                alarmMgr.setDeniedState();
                
                if (secMgr.getState() == STATE_LOCKOUT) {
                    // 3rd consecutive failure — trigger security alarm
                    logger.logEvent("SECURITY", "MAX FAILED ATTEMPTS REACHED! Triggering Alarm & Lockout.");
                    secMgr.setState(STATE_ALARM_ACTIVE);
                    displayMgr.showAlarmScreen();
                } else {
                    displayMgr.showAccessDenied(secMgr.getRemainingAttempts());
                    stateTransitionTimer = millis();
                }
            }
        } 
        else {
            // Digit key typed — add to buffer and update masked display
            if (secMgr.getState() == STATE_LOCKED) {
                secMgr.setState(STATE_ENTERING_PASS);
            }
            displayMgr.showLockedScreen(keypadMgr.getMaskedPassword());
        }
    }

    // -------------------------------------------------------------
    // PRIORITY 3: FSM State Machine Timers & Periodic Housekeeping
    // Each state has specific timing behavior managed via millis().
    // -------------------------------------------------------------
    switch (secMgr.getState()) {

        case STATE_ACCESS_GRANTED:
            // Auto-lock timer: re-engage servo after 5 seconds of unlock
            if (millis() - lockCtrl.getUnlockTimestamp() >= AUTO_LOCK_DELAY_MS) {
                logger.logEvent("TIMED_OUT", "Auto-lock timeout reached (5s). Locking system...");
                lockCtrl.lockLocker();
                alarmMgr.setLockedState();
                secMgr.setState(STATE_LOCKED);
                keypadMgr.resetBuffer();
                displayMgr.showLockedScreen("");
            }
            break;

        case STATE_ACCESS_DENIED:
            // Display "ACCESS DENIED" for 2 seconds, then return to entry screen
            if (millis() - stateTransitionTimer >= 2000) {
                secMgr.setState(STATE_LOCKED);
                keypadMgr.resetBuffer();
                displayMgr.showLockedScreen("");
            }
            break;

        case STATE_ALARM_ACTIVE:
            // Pulse buzzer siren at 300ms intervals (≈1.66Hz) and flash Red LED
            alarmMgr.triggerAlarmPulse();
            displayMgr.showAlarmScreen();
            
            // Check if 30-second lockout penalty has expired
            if (!secMgr.isLockoutActive()) {
                logger.logEvent("SECURITY", "Lockout duration expired. System returning to normal armed state.");
                alarmMgr.stopAlarm();
                secMgr.resetFailedAttempts();
                secMgr.setState(STATE_LOCKED);
                keypadMgr.resetBuffer();
                displayMgr.showLockedScreen("");
            }
            break;

        case STATE_LOCKOUT:
            // Display countdown timer during lockout period
            displayMgr.showLockoutScreen(secMgr.getRemainingLockoutSeconds());
            if (!secMgr.isLockoutActive()) {
                secMgr.setState(STATE_LOCKED);
                secMgr.resetFailedAttempts();
                displayMgr.showLockedScreen("");
            }
            break;

        case STATE_LOCKED:
        case STATE_ENTERING_PASS:
        default:
            // Idle states — no periodic actions required
            break;
    }

    // Periodic UART telemetry broadcast (every 2 seconds)
    logger.logTelemetry(secMgr.getState(), secMgr.getFailedAttempts(), keypadMgr.getMaskedPassword(), lockCtrl.isUnlocked());
}
