#include <msp430g2553.h>
#include "button.h"
#include "led.h"

#define BUTTON BIT3

void configure_button()
{
    P1REN |= BUTTON;                 // enabing pulling
    P1OUT |= BUTTON;                 // pull up
    
    P1IES |= BUTTON;                 // interruption for port3 from 1 to 0
    P1IFG &= ~BUTTON;                // reset interruption flag (to avoid first immidiate call)
    P1IE |= BUTTON;                  // enable intrruptions for bit 3
}

int state = 0;

#pragma vector = PORT1_VECTOR
__interrupt void on_button_pressed()
{
    switch (P1IFG & BUTTON)
    {
        case BUTTON:
            P1IFG &= ~BUTTON;       // reset interruption flag
            if (state == 0)
            {
                led_on();
                state = 1;
            } 
            else 
            {
                led_off();
                state = 0;
            }
            break;
        default:
            P1IFG = 0;
            break;
    }
}
