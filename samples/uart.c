/*
 frequency = 12 Mhz
 baud rate (uart speed) = 9600

 from table (page 424), UCBR0 = 1250
 UCBR0 = UCA0BR0 + UCA0BR1 * 256
 1250 ~ 255 + 4 * 125 = 1279
*/

#include <msp430g2553.h>
#include "uart.h"
#include "servo.h"

#define BT_OUT P1OUT
#define BT_TX BIT1
#define BT_RX BIT2
#define BT_UART_SPEED 0xFF
#define BT_UART_CORRECTION 0x04
#define CMD_BUFFER_SIZE 2
#define BT_RESPONSE_OK 1
#define BT_RESPONSE_FAILED 2

void configure_uart() {
    // use pins as UART
    P1SEL |= BT_RX + BT_TX;
    P1SEL2 |= BT_RX + BT_TX;
    // init pins
    BT_OUT &= ~BT_RX;
    BT_OUT &= ~BT_TX;
    // use sub-main clock
    UCA0CTL1 |= UCSSEL_2;
    // set uart speed
    UCA0BR0 = BT_UART_SPEED;
    UCA0BR1 = BT_UART_CORRECTION;
    // init uart state machine
    UCA0CTL1 &= ~UCSWRST;
    // enable RX interruption
    UC0IE |= UCA0RXIE;
}

int head = 0;
unsigned char uart_rx_buffer[CMD_BUFFER_SIZE];

void message_ready() {
    unsigned char servo_id = uart_rx_buffer[0];
    unsigned char angle = uart_rx_buffer[1];

    if (check_data_range(servo_id, angle)) {
        // apply command
        set_servo_angle(servo_id, angle);
        // send response
        UCA0TXBUF = BT_RESPONSE_OK;
    } else {
        UCA0TXBUF = BT_RESPONSE_FAILED;
    }

    UCA0TXBUF = 1;
    // enable TX interruption
    UC0IE |= UCA0TXIE;
}

void uart_data_received() {
    // collect received byte into buffer
    unsigned char data = UCA0RXBUF;
    uart_rx_buffer[head++] = data;
    // end of command message
    if (head == CMD_BUFFER_SIZE) {
        message_ready();
        head = 0;
    }
}

void uart_data_send() {
    // disable TX interruptions
    UC0IE &= ~UCA0TXIE;
}
