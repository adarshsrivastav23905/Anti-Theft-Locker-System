/**
 * =====================================================================================
 * @file    keypad_manager.cpp
 * @brief   4x4 Matrix Keypad Scanner Implementation
 * @author  Adarsh Srivastav
 * 
 * @details Implements the matrix keypad interface using the Keypad library.
 *          The keypad uses drive-and-sense multiplexing: 4 column pins are
 *          sequentially driven LOW while 4 row pins (with internal pull-ups)
 *          are read. A LOW reading on a row indicates a key press at the
 *          intersection of the active column and detected row.
 * 
 *          Special key functions:
 *          - '*' (asterisk): Clears the entire input buffer
 *          - '#' (hash):     Submits the buffered password for verification
 *          - Digits/letters: Appended to the buffer (up to PASSWORD_LENGTH)
 * =====================================================================================
 */

#include "keypad_manager.h"

/**
 * @brief Constructor — initializes the Keypad library with pin mappings.
 * makeKeymap() converts the 2D char array into the format expected by the library.
 */
KeypadManager::KeypadManager() 
    : customKeypad(makeKeymap(keys), rowPins, colPins, 4, 4), bufferIndex(0) {
    memset(enteredBuffer, 0, sizeof(enteredBuffer));
}

/**
 * @brief Reset the input buffer to empty state.
 * Called during system initialization and after password submission.
 */
void KeypadManager::init() {
    bufferIndex = 0;
    memset(enteredBuffer, 0, sizeof(enteredBuffer));
}

/**
 * @brief Poll the keypad for a single key press (non-blocking).
 * @return The character of the pressed key, or '\0' if no key is pressed.
 */
char KeypadManager::getKey() {
    return customKeypad.getKey();
}

/**
 * @brief Process a keypad input character and update the internal buffer.
 * 
 * @param key           The character received from getKey()
 * @param resultString  Output: current buffer contents as String
 * @param isSubmitted   Output: true if '#' was pressed (submit command)
 * @param isCleared     Output: true if '*' was pressed (clear command)
 * @return true if the key was successfully processed
 */
bool KeypadManager::processInput(char key, String &resultString, bool &isSubmitted, bool &isCleared) {
    isSubmitted = false;
    isCleared = false;

    if (!key) return false;

    if (key == '#') {
        // '#' acts as Enter / Submit Key — finalize buffer and signal submission
        enteredBuffer[bufferIndex] = '\0';
        resultString = String(enteredBuffer);
        isSubmitted = true;
        return true;
    } 
    else if (key == '*') {
        // '*' acts as Clear / Reset Key — wipe entire buffer
        resetBuffer();
        isCleared = true;
        return true;
    } 
    else if (bufferIndex < PASSWORD_LENGTH) {
        // Collect numeric digit or alphanumeric character into buffer
        enteredBuffer[bufferIndex++] = key;
        enteredBuffer[bufferIndex] = '\0';
        resultString = String(enteredBuffer);
        return true;
    }

    // Buffer is full — ignore additional digit input
    return false;
}

/**
 * @brief Clear the password input buffer and reset index to zero.
 */
void KeypadManager::resetBuffer() {
    bufferIndex = 0;
    memset(enteredBuffer, 0, sizeof(enteredBuffer));
}

/**
 * @brief Generate an asterisk-masked representation of the current input.
 * Used for secure OLED display — prevents shoulder surfing.
 * Example: if 3 digits entered, returns "***"
 */
String KeypadManager::getMaskedPassword() const {
    String masked = "";
    for (uint8_t i = 0; i < bufferIndex; i++) {
        masked += "*";
    }
    return masked;
}

/**
 * @brief Retrieve the actual entered characters (unmasked).
 * Used internally for password verification — never displayed on screen.
 */
String KeypadManager::getRawPassword() const {
    return String(enteredBuffer);
}

/**
 * @brief Returns the number of digits currently in the buffer.
 */
uint8_t KeypadManager::getBufferLength() const {
    return bufferIndex;
}
