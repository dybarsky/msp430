#include <msp430g2553.h>
#include "servo.h"

//~

void _configure();

//~

int main(void) {
    _configure();                               // первоначальная настройка контроллера
    _configure_servos();                        // настройка серво-приводов

    _BIS_SR(LPM0_bits);                         // переходим в энергоэффективный режим
}

void _configure() {
    WDTCTL = WDTPW + WDTHOLD;                   // отключаем сторожевой таймер

    BCSCTL1 = CALBC1_1MHZ;                      // используем откалиброваную частоту для 1MHZ
    DCOCTL = CALDCO_1MHZ;
}

//~

#pragma vector = TIMER0_A0_VECTOR
__interrupt void _on_timer_a0_callback(void) {
    OUT_PORT = SERVO;                           // подаём сигнал на пин
    TA0CCTL0 &= ~CCIFG;                         // сбрасываем флан прерывания
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void _on_timer_a1_callback(void) {
    OUT_PORT = 0;                               // выключаем сигнал
    TA0CCTL1 &= ~CCIFG;                         // сбрасываем флан прерывания
}

