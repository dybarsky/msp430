#include <msp430g2553.h>

#define RED 	BIT0
#define GREEN 	BIT6
#define BUTTON 	BIT3

void wait() {
	unsigned int count;
	for (count = 0; count < 60000; count++);
}

void blink(int led) {
	unsigned int count;
	for (count = 0; count < 6; count++) {
		P1OUT ^= led;							// переключаем состояние пина
		wait();
	}
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; 					// отключаем сторожевой таймер
    				
    BCSCTL1 = CALBC1_1MHZ;						// используем откалиброваную частоту для 1MHZ
	DCOCTL = CALDCO_1MHZ;
    
    P1OUT = 0;									// обнуляем регистр
    P1DIR = GREEN + RED;						// устанавливаем на выход пины 0 и 6
    
												// Для LaunchPad версии 1.5 используем резистор
    P1REN |= BUTTON; 							//разрешаем подтяжку 
    P1OUT |= BUTTON; 							//подтяжка вывода P1.3 вверх
        
    while (1) {
		blink(GREEN);							// мигаем зелёным
		
		while ((P1IN & BUTTON) == BUTTON);		// ждём пока не нажмётся кнопка
		
		blink(RED);								// мигаем красным
		
		BCSCTL1 = CALBC1_8MHZ;					// используем откалиброваную частоту для 8MHZ
		DCOCTL = CALDCO_8MHZ;
    }
}
