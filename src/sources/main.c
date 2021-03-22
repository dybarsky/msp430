#include <msp430g2553.h>
#include "servo.h"
#include "button.h"
#include "led.h"

void configure()
{
    WDTCTL = WDTPW + WDTHOLD;                   // turn off watchdog

    BCSCTL1 = CALBC1_12MHZ;                     // use frequency 12 MHz
    DCOCTL = CALDCO_12MHZ;

    P1OUT = 0;                                  // all pins off
    P2OUT = 0;
}


int main(void) 
{
    configure();     
    //configure_led();
    //configure_button();                           
    configure_servo();

    servo_set(0);

    _BIS_SR(LPM0_bits + GIE);                   // low power mode + enable interruptions
}
