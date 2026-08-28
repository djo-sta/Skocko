#include <msp430.h>
#include <stdint.h>
#include "function.h"
#include "sh1106.h"
#include "skocko.h"
/*
GORNJE DUGME JE ZA POTVRDU UNOSA JEDNOG POKUSAJA
DONJE DUGME JE ZA POMERANJE POZICIJE ZNAKA KOJI SE MENJA NA DESNO

LEVI 7SEG PREDSTAVLJA POZICIJU
DESNI 7SEG PREDSTAVLJA BROJ POGADJANJA

P3.0 SDA
P3.1 SCK
*/

volatile uint16_t potentiometer_value = 0;
volatile uint8_t potentiometer_flag = 0;
volatile uint8_t interrupt_flag = 0;
volatile uint8_t a=0;
volatile uint8_t num_of_tries = 0;

volatile uint8_t confirm_flag;
uint8_t finish_flag = 1;
volatile uint8_t last_sign = 0;
volatile uint8_t last_potentiometer_value = 0;

int main(void) {
    
    InitHardware();
    OLED_Init();

    while(1) {          
        MakeWinningCombination();
        PrintCheat();
        while (1) {
            confirm_flag = 1;
            __enable_interrupt();
            do {
                if (potentiometer_flag) {
                    potentiometer_flag = 0;
                    a = (last_sign - last_potentiometer_value + potentiometer_value + 7) % 7;
                    SignInput(a);
                }
                if (interrupt_flag) {
                    interrupt_flag = 0;
                    Refresh();
                }
            } while(confirm_flag);

            __disable_interrupt();
            finish_flag = GuessCheck();
            Saving();
            PrintingResults();
        }
    }
}

void __attribute__ ((interrupt(PORT1_VECTOR))) P1ISR(void) {
    if (P1IFG & BIT5) {
        __delay_cycles(1000);
        if ((P1IN & BIT5) == 0) {
            confirm_flag = 0;
            num_of_tries++;
        }
        P1IFG &= ~BIT5;
    }
    
    if (P1IFG & BIT4) {
        __delay_cycles(1000);
        if ((P1IN & BIT4) == 0) {
            last_sign = NextPosition();
            last_potentiometer_value = potentiometer_value;
            interrupt_flag = 1;
        }
        P1IFG &= ~BIT4;
    }
}

void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR(void) {
    
    switch (ADC12IV) {
        case ADC12IV_ADC12IFG0:
            potentiometer_value = (ADC12MEM0 * 7) / 4096;
            potentiometer_flag = 1;
            interrupt_flag = 1;
            break;
        default:
            break;
    }
}
