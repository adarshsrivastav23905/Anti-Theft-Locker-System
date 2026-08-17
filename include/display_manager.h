/**
 * =====================================================================================
 * @file    display_manager.h
 * @brief   SSD1306 128x64 OLED Display Interface & Screen Renderer
 * @author  Adarsh Srivastav
 * 
 * @details Manages the SSD1306 OLED display via I2C protocol (SDA=GPIO21, SCL=GPIO22).
 *          Provides pre-built screen wireframes for all FSM states:
 *          - Splash screen (boot animation)
 *          - Locked screen with password mask [****]
 *          - Access Granted screen
 *          - Access Denied screen with remaining attempts
 *          - Security Lockout countdown screen
 *          - Intruder Alarm alert screen
 * 
 * @protocol I2C (400kHz Fast Mode, Address 0x3C)
 * @library  Adafruit SSD1306 v2.5.10, Adafruit GFX v1.11.9
 * =====================================================================================
 */

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

class DisplayManager {
private:
    Adafruit_SSD1306 display;                  // SSD1306 driver instance

public:
    DisplayManager();
    bool init();                               // Initialize I2C bus and OLED controller
    void showSplashScreen();                   // Boot splash with project title & author
    void showLockedScreen(const String &maskedPass);  // Password entry screen [****]
    void showAccessGranted();                  // Unlock confirmation with auto-lock notice
    void showAccessDenied(uint8_t remainingAttempts); // Wrong password with attempts left
    void showLockoutScreen(uint32_t remainingSeconds);// Lockout countdown timer display
    void showAlarmScreen();                    // Security breach intruder alert
    void drawHeader(const String &title);      // Reusable header bar with separator line
    void clear();                              // Clear entire display buffer and refresh
};

#endif // DISPLAY_MANAGER_H
