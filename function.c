#include <msp430.h>
#include "function.h"

const unsigned int segtab2[] = {
        0x48,
        0x40,
        0x08,
        0x40,
        0x40,
        0x40,
        0x48,
        0x40,
        0x48,
        0x40
};

const unsigned int segtab3[] = {
        0x80,
        0x00,
        0x80,
        0x80,
        0x00,
        0x80,
        0x80,
        0x80,
        0x80,
        0x80
};

const unsigned int segtab4[] = {
        0x09,
        0x08,
        0x08,
        0x08,
        0x09,
        0x01,
        0x01,
        0x08,
        0x09,
        0x09
};

const unsigned int segtab8[] = {
        0x02,
        0x00,
        0x06,
        0x06,
        0x04,
        0x06,
        0x06,
        0x00,
        0x06,
        0x06
};

void WriteLed(unsigned int digit)
{
        P2OUT |= BIT6 | BIT3;
        P2OUT &= ~segtab2[digit];

        P3OUT |= BIT7;
        P3OUT &= ~segtab3[digit];

        P4OUT |= BIT3 | BIT0;
        P4OUT &= ~segtab4[digit];

        P8OUT |= BIT2 | BIT1;
        P8OUT &= ~segtab8[digit];
}
