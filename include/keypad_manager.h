/**
 * =====================================================================================
 * @file    keypad_manager.h
 * @brief   4x4 Matrix Keypad Scanner & Password Input Buffer Manager
 * @author  Adarsh Srivastav
 * 
 * @details Manages the 4x4 membrane matrix keypad interface using drive-and-sense
 *          row-column scanning. Provides password input buffering, asterisk masking
 *          for secure display, clear (*) and submit (#) key handling, and raw
 *          password retrieval for authentication verification.
 * 
 *          Key Mapping:
 *          | 1 | 2 | 3 | A |
 *          | 4 | 5 | 6 | B |
 *          | 7 | 8 | 9 | C |
 *          | * | 0 | # | D |
 *          
 *          '*' = Clear/Reset input buffer
 *          '#' = Submit/Enter password for verification
 * =====================================================================================
 */

#ifndef KEYPAD_MANAGER_H
#define KEYPAD_MANAGER_H

#include <Keypad.h>
#include "config.h"

class KeypadManager {
private:
    // 4x4 key layout matrix matching physical keypad membrane
    char keys[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
    byte rowPins[4] = {KEYPAD_ROW_1, KEYPAD_ROW_2, KEYPAD_ROW_3, KEYPAD_ROW_4};
    byte colPins[4] = {KEYPAD_COL_1, KEYPAD_COL_2, KEYPAD_COL_3, KEYPAD_COL_4};
    Keypad customKeypad;
    
    char enteredBuffer[PASSWORD_LENGTH + 1];  // Character buffer for entered digits
    uint8_t bufferIndex;                       // Current position in the buffer

public:
    KeypadManager();
    void init();
    char getKey();                             // Poll keypad for a single key press
    bool processInput(char key, String &resultString, bool &isSubmitted, bool &isCleared);
    void resetBuffer();                        // Clear the input buffer
    String getMaskedPassword() const;          // Returns "****" masked representation
    String getRawPassword() const;             // Returns actual entered characters
    uint8_t getBufferLength() const;           // Returns number of digits entered
};

#endif // KEYPAD_MANAGER_H
