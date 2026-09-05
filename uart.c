#include <msp430.h>
#include <stdlib.h>

void UART_SendChar(char c) {
    while (!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = c;
}

void UART_SendString(char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}
