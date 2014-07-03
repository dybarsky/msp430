#include <msp430g2553.h>

void wait() {
	unsigned int count;
	for (count = 0; count < 60000; count++);
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;
    
    P1OUT = 0;
    P1DIR = BIT0 + BIT6;
        
    while (1) {
		
		P1OUT ^= BIT0;
		wait();
        P1OUT ^= BIT6;
        wait();
    }
}
