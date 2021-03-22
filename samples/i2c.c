/*
 frequency = 12 Mhz
 i2c speed = 100kHz

 divider = 12
*/

#include <msp430g2553.h>
#include "i2c.h"
#include "servo.h"

#define BOARD_ADDRESS 0x40;
#define PWM_OUT P1OUT
#define PWM_SDA BIT6
#define PWM_SCL BIT7
#define PWM_DIVIDER 12
#define PWM_BUFFER_SIZE 8

void configure_i2c() {
    // use pins as USCI
    P1SEL |= PWM_SCL + PWM_SDA;
    P1SEL2 |= PWM_SCL + PWM_SDA;
    // init pins
    PWM_OUT &= ~PWM_SCL;
    PWM_OUT &= ~PWM_SDA;
    // reset
    UCB0CTL1 = UCSWRST;
    // I2C Master, synchronous mode
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
    // Use SMCLK & SW reset
    UCB0CTL1 = UCSSEL_2 + UCSWRST;
    UCB0BR0 = PWM_DIVIDER;
    UCB0BR1 = 0;
    UCB0I2CSA = BOARD_ADDRESS;
    // clear SW reset
    UCB0CTL1 &= ~UCSWRST;
    // enable RX/TX interruptions
    IE2 |= UCB0RXIE + UCB0TXIE;
}

int size = 5;
int counter = 0;

unsigned char tx_buffer[PWM_BUFFER_SIZE] = {
    0x6, // LED0_ON_L
    0x1,
//    0x7, // LED0_ON_H
    0x0,
//    0x8, // LED0_OFF_L
    0x1,
//    0x9, // LED0_OFF_H
    0xA
};

void i2c_reset() {

}

void i2c_data_received() {

}

void i2c_data_send() {
    if (counter <= size) {
        UCB0TXBUF = tx_buffer[counter++];
    } else {
        // I2C stop condition
        UCB0CTL1 |= UCTXSTP;
        // Clear USCI_B0 TX int flag
        IFG2 &= ~UCB0TXIFG;
    }
}

void i2c_start_transmition() {
    // ensure stop condition got sent
    while (UCB0CTL1 & UCTXSTP);
    // i2c TX, start condition
    UCB0CTL1 |= UCTR + UCTXSTT;
}

//    if (Rx == 1) { // Master Recieve?
//        PRxData = UCB0RXBUF; // Get RX data
//        __bic_SR_register_on_exit(CPUOFF); // Exit LPM0
//    }
//    else { // Master Transmit
//        if (TXByteCtr) // Check TX byte counter
//        {
//            UCB0TXBUF = WHO_AM_I; // Load TX buffer
//            TXByteCtr--; // Decrement TX byte counter
//        } else {
//            UCB0CTL1 |= UCTXSTP; // I2C stop condition
//            IFG2 &= ~UCB0TXIFG; // Clear USCI_B0 TX int flag
//            __bic_SR_register_on_exit(CPUOFF); // Exit LPM0
//        }
//    }
//void Transmit(void){
//    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
//    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
//    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
//}
//void Receive(void){
//        while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
//        UCB0CTL1 &= ~UCTR ;                     // Clear UCTR
//        UCB0CTL1 |= UCTXSTT;                    // I2C start condition
//        while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
//        UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
//        __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
//}
