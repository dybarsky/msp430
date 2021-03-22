#include <msp430g2553.h>
#include "servo.h"

void _configure_servos() {
    OUT_PORT = 0;                               // обнуляем регистр
    OUT_DIR = SERVO;                            // устанавливаем на выход пин 6

    TACCR0 = PERIOD;                            // за 1/50 сек
    TACCTL0 = CCIE;                             // Разрешаем прерывание таймера по достижению значения CCR0.
    TACCR1 = TIME_AVR;                          // Pulse Duration Modulation - 500 мкс
    TACCTL1 = CCIE;                             // Разрешаем прерывание таймера по достижению значения CCR1.
    TACTL = TASSEL_2 + MC_1 + TACLR;            // sub-mainClock + прямой счёт + инициализация

    _BIS_SR(GIE);                               // разрешаем прерывания
}
