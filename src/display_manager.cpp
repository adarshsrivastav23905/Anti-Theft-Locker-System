/**
 * =====================================================================================
 * @file    display_manager.cpp
 * @brief   SSD1306 128x64 OLED Display Renderer Implementation
 * @author  Adarsh Srivastav
 * 
 * @details Implements the I2C OLED display interface using the Adafruit SSD1306 driver.
 *          Provides distinct screen wireframes for each FSM state:
 * 
 *          Screen Layout (128×64 pixels):
 *          +--------------------+
 *          | === HEADER ===     |  ← Row 0-10: Title bar with horizontal separator
 *          | Status Line 1      |  ← Row 18+: Primary information
 *          | [Large Text Area]  |  ← Row 32+: Size-2 text for emphasis
 *          | Status Line 2      |  ← Row 45+: Secondary details
 *          | Footer / Hint      |  ← Row 54+: User instructions
 *          +--------------------+
 * 
 * @protocol I2C (Wire library, SDA=GPIO21, SCL=GPIO22, Address=0x3C)
 * =====================================================================================
 */

#include "display_manager.h"

/**
 * @brief Constructor — creates the SSD1306 driver with screen dimensions.
 */
DisplayManager::DisplayManager() 
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

/**
 * @brief Initialize the I2C bus and SSD1306 OLED controller.
 * @return true if initialization succeeded, false if I2C communication failed.
 */
bool DisplayManager::init() {
    Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        return false;
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
    return true;
}

/**
 * @brief Draw a standard header bar with title text and horizontal separator.
 * Used as a consistent visual element across all screen states.
 * Format: "=== TITLE ===" with a 1-pixel line at row 10.
 */
void DisplayManager::drawHeader(const String &title) {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("=== ");
    display.print(title);
    display.println(" ===");
    display.drawFastHLine(0, 10, SCREEN_WIDTH, SSD1306_WHITE);
}

/**
 * @brief Display the boot splash screen (shown for 2 seconds at startup).
 * Shows project title, platform info, and author name.
 */
void DisplayManager::showSplashScreen() {
    display.clearDisplay();
    drawHeader("ANTI-THEFT LOCK");
    display.setTextSize(1);
    display.setCursor(10, 20);
    display.println("SECURITY SYSTEM");
    display.setCursor(15, 35);
    display.println("ESP32 / OLED");
    display.setCursor(5, 50);
    display.println("By: Adarsh Srivastav");
    display.display();
}

/**
 * @brief Display the locked/password entry screen.
 * Shows the password mask with entered digits as '*' and remaining as '_'.
 * Example: [**__] means 2 of 4 digits entered.
 * 
 * @param maskedPass The asterisk-masked password string from KeypadManager
 */
void DisplayManager::showLockedScreen(const String &maskedPass) {
    display.clearDisplay();
    drawHeader("LOCKER LOCKED");
    display.setTextSize(1);
    display.setCursor(0, 18);
    display.println("Enter Password:");
    
    // Render password mask in large text: [****] or [**__]
    display.setTextSize(2);
    display.setCursor(20, 32);
    if (maskedPass.length() == 0) {
        display.println("[____]");
    } else {
        display.print("[");
        display.print(maskedPass);
        for (uint8_t i = maskedPass.length(); i < PASSWORD_LENGTH; i++) {
            display.print("_");
        }
        display.println("]");
    }

    // Footer with keypad hint
    display.setTextSize(1);
    display.setCursor(0, 54);
    display.println("*:Clear  #:Enter");
    display.display();
}

/**
 * @brief Display the access granted / locker unlocked confirmation screen.
 * Shows "UNLOCKED" in large text with auto-lock countdown notice.
 */
void DisplayManager::showAccessGranted() {
    display.clearDisplay();
    drawHeader("ACCESS GRANTED");
    display.setTextSize(2);
    display.setCursor(10, 22);
    display.println("UNLOCKED");
    display.setTextSize(1);
    display.setCursor(5, 45);
    display.println("Locker Door Open");
    display.setCursor(5, 55);
    display.println("Auto-lock in 5s...");
    display.display();
}

/**
 * @brief Display the access denied / wrong password screen.
 * Shows "WRONG PASS" in large text with remaining attempt count.
 * 
 * @param remainingAttempts Number of attempts remaining before lockout
 */
void DisplayManager::showAccessDenied(uint8_t remainingAttempts) {
    display.clearDisplay();
    drawHeader("ACCESS DENIED");
    display.setTextSize(2);
    display.setCursor(15, 20);
    display.println("WRONG PASS");
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print("Attempts left: ");
    display.println(remainingAttempts);
    display.display();
}

/**
 * @brief Display the security lockout countdown screen.
 * Shows remaining lockout seconds in large text. Displayed during the
 * 30-second penalty period after 3 consecutive wrong attempts.
 * 
 * @param remainingSeconds Seconds remaining in the lockout period
 */
void DisplayManager::showLockoutScreen(uint32_t remainingSeconds) {
    display.clearDisplay();
    drawHeader("SECURITY LOCKOUT");
    display.setTextSize(1);
    display.setCursor(0, 18);
    display.println("Too many failed tries!");
    display.setCursor(0, 32);
    display.println("System Locked For:");
    display.setTextSize(2);
    display.setCursor(45, 45);
    display.print(remainingSeconds);
    display.println("s");
    display.display();
}

/**
 * @brief Display the security breach / intruder alarm screen.
 * Shows "ALARM ON" in large text with instructions to press the reset button.
 * This screen is displayed continuously during the alarm state.
 */
void DisplayManager::showAlarmScreen() {
    display.clearDisplay();
    drawHeader("!! INTRUDER !!");
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println("ALARM ON");
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.println("SECURITY BREACH!");
    display.setCursor(0, 55);
    display.println("Press Lock Button");
    display.display();
}

/**
 * @brief Clear the entire display buffer and push to screen.
 */
void DisplayManager::clear() {
    display.clearDisplay();
    display.display();
}
