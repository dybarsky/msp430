#include <msp430g2553.h>

#define OUT_PORT	P2OUT
#define OUT_DIR		P2DIR
#define GREEN_LED	BIT3

#define SC_MACRO	100
#define SC_SECOND	5000
#define SC_COUNT	49 // 50 - 1
#define SIZE		5

int count;
int index;
int voltage;

int arr[SIZE] = {10, 30, 50, 70, 90};

//~

void _configure();

//~

int main(void) {
    _configure();								// первоначальная настройка контроллера
    OUT_PORT = GREEN_LED;						// включаем диод
    _BIS_SR(LPM0_bits);							// переходим в энергоэффективный режим
}

void _configure() {
	WDTCTL = WDTPW + WDTHOLD; 					// отключаем сторожевой таймер

    BCSCTL1 = CALBC1_1MHZ;						// используем откалиброваную частоту для 1MHZ
	DCOCTL = CALDCO_1MHZ;

    OUT_PORT = 0;								// обнуляем регистр
    OUT_DIR = GREEN_LED;						// устанавливаем на выход пин 3

	TACCR0 = SC_COUNT;							// за 1/(50*100) сек
	TACCTL0 = CCIE;  							// Разрешаем прерывание таймера по достижению значения CCR0.
	TACTL = TASSEL_2 + ID_2 + MC_1 + TACLR;		// sub-mainClock + делитель частоты 4 + прямой счёт + инициализация

	_BIS_SR(GIE);								// разрешаем прерывания
}

//~

#pragma vector = TIMER0_A0_VECTOR
__interrupt void _on_timer(void) {				// макро-такт - каждое 100-е прерывание (за 1/50 секунды)
	int relativeCount = ++count % SC_MACRO;		// относительное значение счётчика в рамках макро-такта
	if (relativeCount == 0) {					// начало макро-такта
		OUT_PORT = GREEN_LED;					// включаем диод
	}
	if (relativeCount == voltage) {				// % достигнут
		OUT_PORT = ~GREEN_LED;					// выключаем диод
	}
	if (count == SC_SECOND) {					// 1 секунда
		count = 0;
		if (++index == SIZE) index = 0;			// циклический инкремент
		voltage = arr[index];					// следующая частота
	}
}


