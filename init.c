#include "function.h"
#include <msp430.h>
#include <stdint.h>



#define CONV_PERIOD        (2048)  /* 500ms */

void InitHardware() {
    
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // -------------------------
    // Button P1.5
    // -------------------------
    P1DIR &= ~BIT5;
    P1REN |= BIT5;
    P1OUT |= BIT5;
    P1IES |= BIT5;
    P1IFG &= ~BIT5;
    P1IE  |= BIT5;

    // -------------------------
    // Button P1.4
    // -------------------------
    P1DIR &= ~BIT4;
    P1REN |= BIT4;
    P1OUT |= BIT4;
    P1IES |= BIT4;
    P1IFG &= ~BIT4;
    P1IE  |= BIT4;

    // -------------------------
    // Potentiometer
    // -------------------------
    P6SEL |= BIT0;                               // P6.0 = A1
    
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 |= ADC12ON;
    ADC12CTL1 |= ADC12SHS_2 + ADC12SSEL_3 + ADC12CONSEQ_2;
    ADC12CTL2 |= ADC12RES_2;   
    ADC12MCTL0 |= ADC12INCH_0;   
    ADC12CTL0 |= ADC12ENC;      
    ADC12IE |= ADC12IE0; 

    TB0CCR0 = (CONV_PERIOD/2  - 1);
    TB0CCTL0 |= OUTMOD_4; 
    TB0CTL |= TBSSEL__ACLK + ID__8 + MC__UP;
    
    // -------------------------
    // Timer
    // -------------------------

    return;
}
