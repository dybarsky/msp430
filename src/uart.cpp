/*
 * This code uses software serial library for UART transmission
 * TX - P1.1, RX - P1.2
 */

#include <stddef.h>
#include <energia.h>
#include <SoftwareSerial.h>
#include "hardware.h"
#include "uart.h"

void Uart::setup() {
    Serial.begin(BLUETOOTH_BAUD_RATE);
}

bool Uart::data_available() {
    return Serial.available() > 0;
}

uint8_t Uart::read(Package * package) {
    size_t size = Serial.readBytes(buffer, UART_PACKAGE_LENGTH);
    if (size == UART_PACKAGE_LENGTH) {
        package -> servo = buffer[0];
        package -> angle = buffer[1];
        return 1;
    } else {
        return 0;
    }
}

void Uart::write(uint8_t data) {
    Serial.write(data);
}
