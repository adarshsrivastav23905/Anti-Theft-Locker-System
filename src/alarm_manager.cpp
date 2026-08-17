/**
 * =====================================================================================
 * @file    alarm_manager.cpp
 * @brief   Buzzer Siren & LED Status Indicator Implementation
 * @author  Adarsh Srivastav
 * 
 * @details Implements multi-state audio-visual feedback using an active buzzer
 *          and Red/Green LEDs. The alarm siren uses non-blocking millis() timing
 *          to toggle the buzzer and Red LED at 300ms intervals, producing a
 *          pulsing siren effect at approximately 1.66 Hz.
 * 
 *          Siren Frequency Calculation:
 *          f_siren = 1 / (2 × t_pulse) = 1 / (2 × 0.300s) = 1.666 Hz
 * =====================================================================================
 */

#include "alarm_manager.h"

/**
 * @brief Constructor — initializes timing and state tracking variables.
 */
AlarmManager::AlarmManager() : lastPulseTime(0), buzzerState(false) {}

/**
 * @brief Configure GPIO pins for buzzer and LEDs as OUTPUT.
 * All outputs start in LOW (off) state.
 */
void AlarmManager::init() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
}

/**
 * @brief Set indicators for ACCESS GRANTED state.
 * Green LED ON, Red LED OFF, double confirmation beep (60ms × 2).
 */
void AlarmManager::setGrantedState() {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    
    // Play double beep for audible access granted confirmation
    beepShort();
    delay(100);
    beepShort();
}

/**
 * @brief Set indicators for ACCESS DENIED state.
 * Red LED flash with a single sustained error beep (500ms).
 * Green LED forced OFF.
 */
void AlarmManager::setDeniedState() {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);  // 500ms sustained error tone
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
}

/**
 * @brief Set indicators for LOCKED (idle armed) state.
 * All LEDs and buzzer turned OFF — silent standby mode.
 */
void AlarmManager::setLockedState() {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}

/**
 * @brief Non-blocking pulsing siren alarm.
 * Toggles buzzer and Red LED every BUZZER_PULSE_INTERVAL (300ms).
 * Must be called continuously from the main loop during STATE_ALARM_ACTIVE.
 * 
 * The resulting siren frequency: f = 1/(2×300ms) ≈ 1.66 Hz
 */
void AlarmManager::triggerAlarmPulse() {
    digitalWrite(GREEN_LED_PIN, LOW);  // Ensure green is OFF during alarm
    
    if (millis() - lastPulseTime >= BUZZER_PULSE_INTERVAL) {
        lastPulseTime = millis();
        buzzerState = !buzzerState;
        digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
        digitalWrite(RED_LED_PIN, buzzerState ? HIGH : LOW);
    }
}

/**
 * @brief Immediately stop all alarm outputs.
 * Called when the lockout period expires or manual reset button is pressed.
 */
void AlarmManager::stopAlarm() {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    buzzerState = false;
}

/**
 * @brief Play a short tactile feedback beep (60ms).
 * Called on every valid keypad press for user confirmation.
 */
void AlarmManager::beepShort() {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(60);
    digitalWrite(BUZZER_PIN, LOW);
}
