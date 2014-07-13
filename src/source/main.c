#include <msp430g2553.h>

#define OUT_PORT	P2OUT
#define OUT_DIR		P2DIR
#define GREEN_LED	BIT3
#define BUTTON 		BIT3

int count;

//~

void _wait();

void _blink(int led_bit);

void _configure();

//~

int main(void) {

    _configure();								// первоначальная настройка контроллера

    while (1) {
		_blink(GREEN_LED);							// мигаем диодом
    }
}

void _configure() {
	WDTCTL = WDTPW + WDTHOLD; 					// отключаем сторожевой таймер

    BCSCTL1 = CALBC1_1MHZ;						// используем откалиброваную частоту для 1MHZ
	DCOCTL = CALDCO_1MHZ;

    OUT_PORT = 0;								// обнуляем регистр
    OUT_DIR = GREEN_LED;						// устанавливаем на выход пин 3

												// Для LaunchPad версии 1.5 используем резистор
    P1REN |= BUTTON; 							// разрешаем подтяжку
    P1OUT |= BUTTON; 							// подтяжка вывода P1.3 вверх

	TACCR0 = 62500 - 1;							// период 62,500 циклов, от 0 до 62,499.
	TACCTL0 = CCIE;  							// Разрешаем прерывание таймера по достижению значения CCR0.
	TACTL = TASSEL_2 + ID_2 + MC_1 + TACLR;		// sub-mainClock + делитель частоты 8 + прямой счёт + инициализация

	P1IES |= BUTTON;  							// прерывание для 3-го порта по переходу из 1 в 0
	P1IFG &= ~BUTTON; 							// обнуляем флаг прерывания для 3-го бита
												// для предотвращения немедленного срабатывания прерывания
	P1IE |= BUTTON;   							// Разрешаем прерывания для порта 3

	_BIS_SR(GIE);								// разрешаем прерывания
}

//~

#pragma vector = TIMER0_A0_VECTOR
__interrupt void _on_timer(void) {
    // Сброс флага прерывания таймера происходит автоматически, ручное обнуление не нужно
    if (++count > 30) {
		BCSCTL1 = CALBC1_8MHZ;				// используем откалиброваную частоту для 8MHZ
		DCOCTL = CALDCO_8MHZ;
	}
}

#pragma vector = PORT1_VECTOR
__interrupt void _on_button_pressed(void) {
    switch(P1IFG & BIT3) {
        case BIT3:
            P1IFG &= ~BUTTON;  					// обнуляем флаг прерывания для P1.3
            BCSCTL1 = CALBC1_1MHZ;				// используем откалиброваную частоту для 8MHZ
			DCOCTL = CALDCO_1MHZ;
			count = 0;
            return;
        default:
            P1IFG = 0;   						// Возможно в этом нет необходимости, но обнуляем
												// флаги всех прерываний в P1, на всякий случай.
												// Хотя лучше было бы добавить обработчик ошибки.
            return;
    }
}

//~

void _wait() {
	unsigned int count;
	for (count = 0; count < 60000; count++);
}

void _blink(int led) {
	unsigned int count;
	for (count = 0; count < 6; count++) {
		OUT_PORT ^= led;							// переключаем состояние пина
		_wait();
	}
}
