/**
 * =====================================================================================
 * @file    serial_monitor.h
 * @brief   UART Serial Telemetry Logger & Debug Output Formatter
 * @author  Adarsh Srivastav
 * 
 * @details Provides structured UART serial output at 115200 baud for real-time
 *          system monitoring and debugging. Outputs two types of messages:
 * 
 *          1. Event Logs: Tagged one-time messages for state transitions
 *             Format: [TAG] Message text
 *             Example: [AUTH] ACCESS GRANTED! Correct password.
 * 
 *          2. Periodic Telemetry: System status broadcast every 2 seconds
 *             Format: [TELEMETRY] State: X | FailedAttempts: N | InputBuf: *** | ServoUnlocked: YES/NO
 * 
 * @baud    115200 bps
 * @interval 2000ms telemetry refresh period
 * =====================================================================================
 */

#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include <Arduino.h>
#include "config.h"

class SerialMonitor {
private:
    uint32_t lastTelemetryTime;    // millis() timestamp of last telemetry broadcast

public:
    SerialMonitor();
    void init(uint32_t baudRate = 115200);  // Initialize UART and print boot banner
    void logEvent(const String &tag, const String &message);  // Log tagged event message
    void logTelemetry(LockerState state, uint8_t failedAttempts, const String &maskedPass, bool isUnlocked);
};

#endif // SERIAL_MONITOR_H
