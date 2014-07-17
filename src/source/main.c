#include <msp430g2553.h>

#define OUT_PORT    P1OUT
#define OUT_DIR     P1DIR
#define SERVO       BIT6

#define PERIOD      20000
#define TIME_MIN    500
#define TIME_MAX    2400

//~

void _configure();

//~

int main(void) {
    _configure();                               // первоначальная настройка контроллера
    _BIS_SR(LPM0_bits);                         // переходим в энергоэффективный режим
}

void _configure() {
    WDTCTL = WDTPW + WDTHOLD;                   // отключаем сторожевой таймер

    BCSCTL1 = CALBC1_1MHZ;                      // используем откалиброваную частоту для 1MHZ
    DCOCTL = CALDCO_1MHZ;

    OUT_PORT = 0;                               // обнуляем регистр
    OUT_DIR = SERVO;                            // устанавливаем на выход пин 6

    TACCR0 = PERIOD;                            // за 1/50 сек
    TACCTL0 = CCIE;                             // Разрешаем прерывание таймера по достижению значения CCR0.
    TACCR1 = TIME_MIN;                          // Pulse Duration Modulation - 500 мкс
    TACCTL1 = CCIE;                             // Разрешаем прерывание таймера по достижению значения CCR1.
    TACTL = TASSEL_2 + MC_1 + TACLR;            // sub-mainClock + прямой счёт + инициализация

    _BIS_SR(GIE);                               // разрешаем прерывания
}

//~

#pragma vector = TIMER0_A0_VECTOR
__interrupt void _on_timer_pwm_start(void) {
    OUT_PORT = SERVO;                           // подаём сигнал на пин
    TA0CCTL0 &= ~CCIFG;                         // сбрасываем флан прерывания
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void _on_timer_pwm_stop(void) {
    OUT_PORT = 0;                               // выключаем сигнал
    TA0CCTL1 &= ~CCIFG;                         // сбрасываем флан прерывания
}

