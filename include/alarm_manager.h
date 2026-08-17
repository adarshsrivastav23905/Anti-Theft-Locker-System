/**
 * =====================================================================================
 * @file    alarm_manager.h
 * @brief   Buzzer Siren & LED Status Indicator Controller
 * @author  Adarsh Srivastav
 * 
 * @details Manages the active buzzer and Red/Green LED indicators for multi-state
 *          feedback. Provides distinct audio-visual patterns for each system state:
 *          
 *          - Access Granted: Green LED ON, double short beep (60ms × 2)
 *          - Access Denied:  Red LED flash, single long beep (500ms)
 *          - Locked State:   All indicators OFF
 *          - Alarm Active:   Pulsing siren (300ms toggle ≈ 1.66Hz), Red LED flashing
 *          - Key Feedback:   Short tactile beep (60ms) on each keypress
 * 
 * @pins    Buzzer=GPIO15, Red LED=GPIO2 (330Ω), Green LED=GPIO4 (330Ω)
 * =====================================================================================
 */

#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include <Arduino.h>
#include "config.h"

class AlarmManager {
private:
    uint32_t lastPulseTime;        // millis() timestamp of last siren toggle
    bool buzzerState;              // Current buzzer ON/OFF state for pulse cycling

public:
    AlarmManager();
    void init();                   // Configure GPIO pins as OUTPUT, set initial LOW
    void setGrantedState();        // Green LED ON, double confirmation beep
    void setDeniedState();         // Red LED flash, single error beep (500ms)
    void setLockedState();         // All indicators OFF (default armed state)
    void triggerAlarmPulse();      // Non-blocking 300ms pulsing siren + Red LED
    void stopAlarm();              // Immediately silence buzzer and clear all LEDs
    void beepShort();              // 60ms tactile keypress feedback beep
};

#endif // ALARM_MANAGER_H
