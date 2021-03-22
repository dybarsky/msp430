#include <energia.h>
#include "hardware.h"
#include "servo.h"
#include "uart.h"

#define ADDRESS     PCA9685_I2C_ADDRESS
#define FREQUENCY   SERVO_PWM_FREQUENCY
#define SIZE        SERVO_QUANTITY

Uart uart;
Pwm pwm(ADDRESS, FREQUENCY);
Servo * servos[SIZE];
Package package;

void setup() {
    // periphery controllers
    pwm.setup();
    uart.setup();
    // servos controllers
    for (char i = 0; i < SERVO_QUANTITY; i ++) {
        servos[i] = new Servo(i, &pwm);
    }
}

uint8_t * readDummy() {
    static uint8_t array[4];
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    array[3] = 4;
    return array;
}
void move();
void read_from_uart();
void read_from_pwm();

void loop() {
    //move();
    read_from_uart();
}

void move() {
    uint8_t min = 0;
    uint8_t max = 180;
    uint8_t angle = min;
    uint8_t step = 30;
    bool up = 1;
    while (true) {
        if (angle + step >= max) up = 0;
        if (angle - step <= min) up = 1;
        if (up == 1) {
            angle = angle + step;
        } else {
            angle = angle - step;
        }
        
        servos[2] -> set_angle(angle);
        servos[1] -> set_angle(angle);
        //servos[0] -> set_angle(angle);
        delay(1000);
    }
}

void read_from_uart() {
    if (uart.data_available()) {
        uint8_t result = uart.read(&package);
        if (result != 0) {
            servos[package.servo] -> set_angle(package.angle);
        }
    }
}

void read_from_pwm() {
    uint8_t array[4];
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    array[3] = 4;

    //uint8_t * data = read();
    uint8_t * data = pwm.read();

    for (int i = 0; i < 12; i++) {
        //uint8_t value = array[i];
        uint8_t value = *(data + i);
        uart.write(value);
    }
}


