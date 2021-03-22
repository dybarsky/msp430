/*
 * This code controls PCA9685 PWM board over I2C protocol using Wire library
 */

#include <energia.h>
#include <Wire.h>
#include "pwm.h"

#define PWM_TICKS 4096

#define PCA9685_REG_MODE1 0x0
#define PCA9685_REG_LED0_ON_L 0x6
#define PCA9685_REG_LED0_ON_H 0x7
#define PCA9685_REG_LED0_OFF_L 0x8
#define PCA9685_REG_LED0_OFF_H 0x9

#define PCA9685_MASK 0x7f   // 8th bit 0, all bits 1
#define PCA9685_SLEEP 0x10  // 5th bit 1, all bits 0
#define PCA9685_25Mhz_TICKS 25000000
#define PCA9685_PRESCALE 0xFE

uint8_t calculate_prescale(float frequency) {
    frequency *= 0.9;
    uint8_t prescale = floor(PCA9685_25Mhz_TICKS / PWM_TICKS / frequency - 0.5);
    return prescale;
}

Pwm::Pwm(uint8_t i2c_address, uint8_t frequency) {
    this -> i2c_address = i2c_address;
    this -> frequency = frequency;
}

void Pwm::setup() {
    Wire.setModule(1);
    Wire.begin();
    write_byte(PCA9685_REG_MODE1, 0x0);
    set_frequency(frequency);
}

void Pwm::set_pwm(uint8_t channel, uint16_t duty_time) {
    Wire.beginTransmission(i2c_address);
    Wire.write(PCA9685_REG_LED0_ON_L + 4 * (channel + 4));
    Wire.write(0);
    Wire.write(0);
    Wire.write(duty_time & 0xFF);
    Wire.write(duty_time >> 8);
    Wire.endTransmission();
}

void Pwm::set_frequency(float frequency) {
    uint8_t oldMode = read_byte(PCA9685_REG_MODE1);
    uint8_t newMode = (oldMode & PCA9685_MASK) | PCA9685_SLEEP;
    // go to sleep
    write_byte(PCA9685_REG_MODE1, newMode);
    // set prescale
    write_byte(PCA9685_PRESCALE, calculate_prescale(frequency));
    // restore old mode
    write_byte(PCA9685_REG_MODE1, oldMode);
    delay(5);
     // auto increment bit + restart
    write_byte(PCA9685_REG_MODE1, oldMode | 0xa1);
}

uint8_t * Pwm::read() {
    static uint8_t array[12];
    array[0] = read_byte(0x6);
    array[1] = read_byte(0x7);
    array[2] = read_byte(0x8);
    array[3] = read_byte(0x9);

    array[4] = read_byte(0xa);
    array[5] = read_byte(0xb);
    array[6] = read_byte(0xc);
    array[7] = read_byte(0xd);

    array[8] = read_byte(0xe);
    array[9] = read_byte(0xf);
    array[10] = read_byte(0x10);
    array[11] = read_byte(0x11) >> 8;

    return array;
}

uint8_t Pwm::read_byte(uint8_t address) {
    Wire.beginTransmission(i2c_address);
    Wire.write(address);
    Wire.endTransmission();

    Wire.beginTransmission(i2c_address);
    Wire.requestFrom((uint8_t) i2c_address, (uint8_t) 1);
    uint8_t data = Wire.read();
    Wire.endTransmission();
    return data;
}

void Pwm::write_byte(uint8_t address, uint8_t value) {
    Wire.beginTransmission(i2c_address);
    Wire.write(address);
    Wire.write(value);
    Wire.endTransmission();
}
