#include <msp430g2553.h>
#include "led.h"

#define LED_PIN BIT0

void configure_led() {
    P1DIR |= LED_PIN;
}

void led_on() {
    P1OUT |= LED_PIN;
}

void led_off() {
    P1OUT &= ~LED_PIN;
}