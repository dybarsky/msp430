#include <msp430g2553.h>
#include "servo.h"

#define OUT_PORT    P1OUT
#define OUT_DIR     P1DIR
#define SERVO       BIT6

#define PERIOD      20000
#define TIME_MIN    500
#define TIME_MAX    2400
#define TIME_AVR    1450

void configure_servo()
{
    OUT_PORT = 0;                               // turn off port
    OUT_DIR = SERVO;                            // setup pin output

    TACCR0 = PERIOD;                            // 1/50 sec
    TACCTL0 = CCIE;                             // enable interruption of timer when CCR0 is reached
    TACCR1 = TIME_AVR;                          // pulse duration modulation - 500 ms
    TACCTL1 = CCIE;                             // enable interruption of timer when CCR1 is reached
    TACTL = TASSEL_2 + MC_1 + TACLR;            // sub-mainClock + прямой счёт + init
}

void servo_max()
{
    TACCR1 = TIME_MAX;
}

void servo_min()
{
    TACCR1 = TIME_MIN;
}

void servo_mid()
{
    TACCR1 = TIME_AVR;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void on_timer_a0_callback(void) {
    OUT_PORT = SERVO;                           // turn on port
    TA0CCTL0 &= ~CCIFG;                         // reset interruption flag
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void on_timer_a1_callback(void) 
{
    OUT_PORT = 0;                               // turn off port
    TA0CCTL1 &= ~CCIFG;                         // reset interruption flag
}
