#include "msp430g2553.h"
#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT1
#define RXD BIT2

const char string[] = { "Hello World\r\n" };
unsigned int i; //Counter

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
    DCOCTL = 0;                 // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_12MHZ;      // Set DCO
    DCOCTL = CALDCO_12MHZ;

    P2DIR = 0xFF;           // All P2.x outputs
    P2OUT &= 0x00;          // All P2.x reset
    P1SEL |= RXD + TXD ;    // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= RXD + TXD ;   // P1.1 = RXD, P1.2=TXD
    P1DIR |= RXLED + TXLED;
    P1OUT &= 0x00;

    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 = 0xFF;     // 12MHz 9600
    UCA0BR1 = 0x04;     // 4 * 256 = 1024
    UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt

    _BIS_SR(LPM0_bits + GIE);
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_RECEIVE(void)
{
   P1OUT |= RXLED;
   if (UCA0RXBUF == 'a') // 'a' received?
   {
      i = 0;
      UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
      UCA0TXBUF = string[i++];
   }
   P1OUT &= ~RXLED;
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void UART_SEND(void)
{
   P1OUT |= TXLED;
   UCA0TXBUF = string[i++]; // TX next character
   if (i == sizeof string - 1) // TX over?
      UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
   P1OUT &= ~TXLED;
}
