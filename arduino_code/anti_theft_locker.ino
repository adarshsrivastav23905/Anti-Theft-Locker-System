/*
 * =====================================================================================
 * PROJECT TITLE: Anti-Theft Locker System with Keypad & OLED
 * AUTHOR: Adarsh Srivastav
 * PLATFORM: ESP32 DevKit V4 / Arduino IDE
 * HARDWARE: 4x4 Matrix Keypad, 128x64 SSD1306 OLED, Servo Motor, Active Buzzer, LEDs
 * =====================================================================================
 */

#include <Wire.h>
#include <Keypad.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------------------------------------------------------------
// PIN DEFINITIONS
// -------------------------------------------------------------
#define OLED_SDA_PIN        21
#define OLED_SCL_PIN        22
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define OLED_RESET          -1
#define OLED_I2C_ADDRESS    0x3C

#define KEYPAD_ROW_1        13
#define KEYPAD_ROW_2        12
#define KEYPAD_ROW_3        14
#define KEYPAD_ROW_4        27

#define KEYPAD_COL_1        26
#define KEYPAD_COL_2        25
#define KEYPAD_COL_3        33
#define KEYPAD_COL_4        32

#define SERVO_PIN           18
#define BUZZER_PIN          15
#define RED_LED_PIN         2
#define GREEN_LED_PIN       4
#define LOCK_BUTTON_PIN     5

// -------------------------------------------------------------
// CONFIGURATION CONSTANTS
// -------------------------------------------------------------
#define DEFAULT_PASSWORD    "1234"
#define PASSWORD_LENGTH     4
#define MAX_FAILED_ATTEMPTS 3
#define AUTO_LOCK_DELAY_MS  5000
#define LOCKOUT_DURATION_MS 30000

// -------------------------------------------------------------
// HARDWARE INSTANCES
// -------------------------------------------------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo lockerServo;

const char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[4] = {KEYPAD_ROW_1, KEYPAD_ROW_2, KEYPAD_ROW_3, KEYPAD_ROW_4};
byte colPins[4] = {KEYPAD_COL_1, KEYPAD_COL_2, KEYPAD_COL_3, KEYPAD_COL_4};
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

// -------------------------------------------------------------
// GLOBAL STATE VARIABLES
// -------------------------------------------------------------
String enteredPass = "";
uint8_t failedAttempts = 0;
bool isUnlocked = false;
bool isAlarmActive = false;
uint32_t unlockTime = 0;
uint32_t lockoutStartTime = 0;
uint32_t lastPulseTime = 0;
bool buzzerState = false;

// -------------------------------------------------------------
// FUNCTION PROTOTYPES
// -------------------------------------------------------------
void drawHeader(const String &title);
void showLockedScreen();
void showAccessGranted();
void showAccessDenied();
void showAlarmScreen();
void unlockLocker();
void lockLocker();
void beepShort();

void setup() {
  Serial.begin(115200);
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(LOCK_BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);

  lockerServo.attach(SERVO_PIN);
  lockLocker();

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("[ERROR] OLED init failed");
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  drawHeader("ANTI-THEFT LOCK");
  display.setCursor(10, 25);
  display.println("System Starting...");
  display.display();
  delay(2000);

  showLockedScreen();
  Serial.println("[SYS] Anti-Theft Locker System Armed.");
}

void loop() {
  // Manual Reset Button Check
  if (digitalRead(LOCK_BUTTON_PIN) == LOW) {
    lockLocker();
    isAlarmActive = false;
    failedAttempts = 0;
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    enteredPass = "";
    showLockedScreen();
    delay(300);
    return;
  }

  // Handle Alarm Pulsing if Breached
  if (isAlarmActive) {
    if (millis() - lastPulseTime >= 300) {
      lastPulseTime = millis();
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
      digitalWrite(RED_LED_PIN, buzzerState ? HIGH : LOW);
    }
    showAlarmScreen();

    if (millis() - lockoutStartTime >= LOCKOUT_DURATION_MS) {
      isAlarmActive = false;
      failedAttempts = 0;
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      showLockedScreen();
    }
    return;
  }

  // Handle Auto-lock Timer
  if (isUnlocked && (millis() - unlockTime >= AUTO_LOCK_DELAY_MS)) {
    lockLocker();
    digitalWrite(GREEN_LED_PIN, LOW);
    showLockedScreen();
    Serial.println("[AUTO] Auto-lock engaged.");
  }

  // Keypad Reading
  char key = customKeypad.getKey();
  if (key) {
    beepShort();
    
    if (key == '#') { // Submit
      if (enteredPass == DEFAULT_PASSWORD) {
        Serial.println("[AUTH] Password Correct!");
        failedAttempts = 0;
        unlockLocker();
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        showAccessGranted();
      } else {
        Serial.println("[AUTH] Wrong Password!");
        failedAttempts++;
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
        showAccessDenied();
        delay(1500);
        digitalWrite(RED_LED_PIN, LOW);

        if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
          isAlarmActive = true;
          lockoutStartTime = millis();
          Serial.println("[ALARM] Maximum failed attempts! Alarm triggered.");
        } else {
          showLockedScreen();
        }
      }
      enteredPass = "";
    } else if (key == '*') { // Clear
      enteredPass = "";
      showLockedScreen();
    } else if (enteredPass.length() < PASSWORD_LENGTH) {
      enteredPass += key;
      showLockedScreen();
    }
  }
}

void drawHeader(const String &title) {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("=== ");
  display.print(title);
  display.println(" ===");
  display.drawFastHLine(0, 10, SCREEN_WIDTH, SSD1306_WHITE);
}

void showLockedScreen() {
  display.clearDisplay();
  drawHeader("LOCKER LOCKED");
  display.setTextSize(1);
  display.setCursor(0, 18);
  display.println("Enter Password:");
  display.setTextSize(2);
  display.setCursor(20, 32);
  display.print("[");
  for (uint8_t i = 0; i < enteredPass.length(); i++) display.print("*");
  for (uint8_t i = enteredPass.length(); i < PASSWORD_LENGTH; i++) display.print("_");
  display.println("]");
  display.setTextSize(1);
  display.setCursor(0, 54);
  display.println("*:Clear  #:Enter");
  display.display();
}

void showAccessGranted() {
  display.clearDisplay();
  drawHeader("ACCESS GRANTED");
  display.setTextSize(2);
  display.setCursor(10, 22);
  display.println("UNLOCKED");
  display.setTextSize(1);
  display.setCursor(5, 45);
  display.println("Locker Door Open");
  display.display();
}

void showAccessDenied() {
  display.clearDisplay();
  drawHeader("ACCESS DENIED");
  display.setTextSize(2);
  display.setCursor(15, 20);
  display.println("WRONG PASS");
  display.setTextSize(1);
  display.setCursor(0, 45);
  display.print("Attempts left: ");
  display.println(MAX_FAILED_ATTEMPTS - failedAttempts);
  display.display();
}

void showAlarmScreen() {
  display.clearDisplay();
  drawHeader("!! INTRUDER !!");
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("ALARM ON");
  display.setTextSize(1);
  display.setCursor(0, 45);
  display.println("SECURITY BREACH!");
  display.display();
}

void unlockLocker() {
  lockerServo.write(90);
  isUnlocked = true;
  unlockTime = millis();
}

void lockLocker() {
  lockerServo.write(0);
  isUnlocked = false;
}

void beepShort() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(60);
  digitalWrite(BUZZER_PIN, LOW);
}
