#include <msp430g2553.h>

#define OUT_PORT	P2OUT
#define OUT_DIR		P2DIR
#define GREEN_LED	BIT3

int count;

//~

void _wait();

void _blink(int led_bit);

void _configure();

//~

int main(void) {
    _configure();								// первоначальная настройка контроллера
}

void _configure() {
	WDTCTL = WDTPW + WDTHOLD; 					// отключаем сторожевой таймер

    BCSCTL1 = CALBC1_1MHZ;						// используем откалиброваную частоту для 1MHZ
	DCOCTL = CALDCO_1MHZ;

    OUT_PORT = 0;								// обнуляем регистр
    OUT_DIR = GREEN_LED;						// устанавливаем на выход пин 3

	TACCR0 = 62500 - 1;							// за 0.5 сек
	TACCTL0 = CCIE;  							// Разрешаем прерывание таймера по достижению значения CCR0.
	TACTL = TASSEL_2 + ID_3 + MC_1 + TACLR;		// sub-mainClock + делитель честоты 8 + прямой счёт + инициализация

	_BIS_SR(LPM0_bits + GIE);								// разрешаем прерывания
}

//~

#pragma vector = TIMER0_A0_VECTOR
__interrupt void _on_timer(void) {
	if (++count == 3) count = 0;
	switch (count) {
		case 0 :
			OUT_PORT = GREEN_LED;
			break;
		case 1:
			OUT_PORT = ~GREEN_LED;
			break;
	}
}
