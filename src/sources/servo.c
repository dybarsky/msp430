/*
 frequency = 12 Mhz
 divider = 4
 clock rate = 3 000 000

 pwm frequency = 50 Hz (1/50 sec)
 pwm period = 3 000 000 / 50 = 60 000
*/


#include <msp430g2553.h>
#include "servo.h"

#define OUT_PORT    P1OUT
#define OUT_DIR     P1DIR
#define SERVO       BIT6

#define PWM_PERIOD_TIME 60000
#define PWM_MAX_TIME 7200           // 2400 microsec * 3
#define PWM_MIN_TIME 1500           // 500 microsec * 3
#define ONE_DEGREE_TIME 32          // (7200 - 1500) / 180

void configure_servo()
{
    OUT_PORT = 0;                               // turn off port
    OUT_DIR = SERVO;                            // setup pin output

    TACCR0 = PWM_PERIOD_TIME;                   // pwm period
    TACCR1 = PWM_MIN_TIME;                      // pwm duty part
    TACCTL0 = CCIE;                             // enable interruption of timer when CCR0 is reached
    TACCTL1 = CCIE;                             // enable interruption of timer when CCR1 is reached
    TACTL = TASSEL_2 + ID_2 + MC_1 + TACLR;     // sub-mainClock + freq divider 4 + up mode + init
}

void servo_set(int angle)
{
    int time = angle * ONE_DEGREE_TIME + PWM_MIN_TIME;
    TACCR1 = time;
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
