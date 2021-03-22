/*
 * This code controls servo angle using Pwm
 *
 * min servo time = 500 us
 * max servo time = 2400 us
 * ticks per second = 4096 * 50 = 204800
 * min ticks = 204800 / 1000000 * 500 = 103
 * max ticks = 204800 / 1000000 * 2400 = 492
 * one degree time = (492 - 103) / 180 = ~2
 */

#include "servo.h"

#define MIN_TIME 103
#define MAX_TIME 492
#define ONE_DEGREE_TIME 2

uint16_t angle_to_pwm(uint8_t angle) {
    return MIN_TIME + ONE_DEGREE_TIME * angle;
}

Servo::Servo(uint8_t id, Pwm *pwm) {
    this -> id = id;
    this -> pwm = pwm;
}

void Servo::set_angle(uint8_t angle) {
    uint16_t pwm_time = angle_to_pwm(angle);
    pwm -> set_pwm(id, pwm_time);
}

void Servo::max_angle() {
    pwm -> set_pwm(id, MAX_TIME);
}

void Servo::min_angle() {
    pwm -> set_pwm(id, MIN_TIME);
}
