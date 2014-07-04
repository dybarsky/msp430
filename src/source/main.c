#include <msp430g2553.h>

#define RED 	BIT0
#define GREEN 	BIT6
#define BUTTON 	BIT3

//~

void wait();

void blink(int led_bit);

//~

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; 					// отключаем сторожевой таймер

    BCSCTL1 = CALBC1_1MHZ;						// используем откалиброваную частоту для 1MHZ
	DCOCTL = CALDCO_1MHZ;

    P1OUT = 0;									// обнуляем регистр
    P1DIR = GREEN;								// устанавливаем на выход пины 0 и 6

												// Для LaunchPad версии 1.5 используем резистор
    P1REN |= BUTTON; 							// разрешаем подтяжку
    P1OUT |= BUTTON; 							// подтяжка вывода P1.3 вверх

	P1IES |= BIT3;  							// прерывание для 3-го порта по переходу из 1 в 0
	P1IFG &= ~BIT3; 							// обнуляем флаг прерывания для 3-го бита
												// для предотвращения немедленного срабатывания прерывания
	P1IE |= BIT3;   							// Разрешаем прерывания для порта 3

	_BIS_SR(GIE);								// разрешаем прерывания

    while (1) {
		blink(GREEN);							// мигаем зелёным
    }
}

//~

#pragma vector = PORT1_VECTOR
__interrupt void _on_button_pressed(void) {
    switch(P1IFG & BIT3) {
        case BIT3:
            P1IFG &= ~BIT3;    					// обнуляем флаг прерывания для P1.3
            BCSCTL1 = CALBC1_8MHZ;				// используем откалиброваную частоту для 8MHZ
			DCOCTL = CALDCO_8MHZ;
            return;
        default:
            P1IFG = 0;   						// Возможно в этом нет необходимости, но обнуляем
												// флаги всех прерываний в P1, на всякий случай.
												// Хотя лучше было бы добавить обработчик ошибки.
            return;
    }
}

//~

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
