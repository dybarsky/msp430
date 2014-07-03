#include <msp430g2553.h>

#define RED BIT0
#define GREEN BIT6
#define BUTTON BIT3

void wait() {
	unsigned int count;
	for (count = 0; count < 60000; count++);
}

void blink(int led) {
	unsigned int count;
	for (count = 0; count < 6; count++) {
		P1OUT ^= led;
		wait();
	}
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // отключаем сторожевой таймер
    
    // используем откалиброваную частоту для 1MHZ
    BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
    
    P1OUT = 0;
    P1DIR = GREEN + RED;
    
    // Для LaunchPad версии 1.5 используем резистор
    P1REN |= BUTTON; //разрешаем подтяжку 
    P1OUT |= BUTTON; //подтяжка вывода P1.3 вверх
        
    while (1) {
		blink(GREEN);
		while ((P1IN & BUTTON) == BUTTON);
		blink(RED);
		// используем откалиброваную частоту для 8MHZ
		BCSCTL1 = CALBC1_8MHZ;
		DCOCTL = CALDCO_8MHZ;
    }
}
