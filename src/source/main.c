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
    _BIS_SR(LPM0_bits);							// переходим в энергоэффективный режим
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

	_BIS_SR(GIE);								// разрешаем прерывания
}

//~

#pragma vector = TIMER0_A0_VECTOR
__interrupt void _on_timer(void) {
	if (++count == 3) count = 0;				//	используются значения от 0 до 3
	switch (count) {
		case 0 :
			OUT_PORT = GREEN_LED;				// на 0 выключаем; t=0.5s
			break;
		case 1:
			OUT_PORT = ~GREEN_LED;				// на 1 включаем (на 2 уже включено); t=1s
			break;
	}
}
