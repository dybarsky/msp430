#include <msp430g2553.h>
#include "servo.h"
//#include "uart.h"
//#include "i2c.h"

void configure() {
    // turn off watchdog
    WDTCTL = WDTPW + WDTHOLD;

    // use frequency 12 MHz
    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;

    P1OUT = 0;
    P2OUT = 0;
}

int main(void) {
    // initial mcu config
    configure();

    // config periphery
    // configure_uart();
    // configure_servos();
    // configure_i2c();

    // for test purpose
    // i2c_reset();
    // i2c_start_transmition();

    // testing PWM servo
    configure_servos();
    set_servo_angle(0, 180);

    // low power mode
    _BIS_SR(LPM0_bits);
}

// #pragma vector = USCIAB0RX_VECTOR
// __interrupt void usci_rx_isr(void) {
//     uart_data_received();
// }

// #pragma vector = USCIAB0TX_VECTOR
// __interrupt void usci_tx_isr(void) {
//     // uart_data_send();
//     i2c_data_send();
// }
