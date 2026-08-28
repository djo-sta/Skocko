#include <msp430.h>
#include "sh1106.h"

static uint8_t framebuffer[SH1106_WIDTH * SH1106_HEIGHT / 8];

static uint8_t x0 = 0;
static uint8_t y0 = 0;

static const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00},     //SPACE     0

    {0x7E, 0x11, 0x11, 0x7E, 0x00},     //A         1
    {0x7F, 0x49, 0x49, 0x36, 0x00},     //B         2
    {0x3E, 0x41, 0x41, 0x22, 0x00},     //C         3
    {0x7F, 0x41, 0x41, 0x3E, 0x00},     //D         4
    {0x7F, 0x49, 0x49, 0x41, 0x00},     //E         5
    {0x7F, 0x09, 0x09, 0x01, 0x00},     //F         6
    {0x3E, 0x41, 0x49, 0x7A, 0x00},     //G         7
    {0x7F, 0x08, 0x08, 0x7F, 0x00},     //H         8
    {0x00, 0x41, 0x7F, 0x41, 0x00},     //I         9
    {0x20, 0x40, 0x41, 0x3F, 0x00},     //J         10
    {0x7F, 0x08, 0x14, 0x63, 0x00},     //K         11
    {0x7F, 0x40, 0x40, 0x40, 0x00},     //L         12
    {0x7F, 0x02, 0x04, 0x02, 0x7F},     //M         13
    {0x7F, 0x02, 0x04, 0x08, 0x7F},     //N         14
    {0x3E, 0x41, 0x41, 0x3E, 0x00},     //O         15
    {0x7F, 0x09, 0x09, 0x06, 0x00},     //P         16
    {0x3E, 0x41, 0x51, 0x7E, 0x00},     //Q         17
    {0x7F, 0x09, 0x19, 0x66, 0x00},     //R         18
    {0x26, 0x49, 0x49, 0x32, 0x00},     //S         19
    {0x01, 0x7F, 0x01, 0x01, 0x00},     //T         20
    {0x3F, 0x40, 0x40, 0x3F, 0x00},     //U         21
    {0x1F, 0x20, 0x40, 0x20, 0x1F},     //V         22
    {0x3F, 0x40, 0x30, 0x40, 0x3F},     //W         23
    {0x63, 0x14, 0x08, 0x14, 0x63},     //X         24
    {0x07, 0x08, 0x70, 0x08, 0x07},     //Y         25
    {0x61, 0x51, 0x49, 0x45, 0x43},     //Z         26

    {0x3E, 0x41, 0x41, 0x41, 0x3E},     //0         27
    {0x00, 0x00, 0x7F, 0x00, 0x00},     //1         28
    {0x62, 0x51, 0x49, 0x49, 0x46},     //2         29
    {0x22, 0x41, 0x49, 0x49, 0x36},     //3         30
    {0x18, 0x14, 0x12, 0x7F, 0x10},     //4         31
    {0x2F, 0x49, 0x49, 0x49, 0x31},     //5         32
    {0x3E, 0x49, 0x49, 0x49, 0x32},     //6         33
    {0x01, 0x01, 0x71, 0x09, 0x07},     //7         34
    {0x36, 0x49, 0x49, 0x49, 0x36},     //8         35
    {0x26, 0x49, 0x49, 0x49, 0x3E},     //9         36

    {0x04, 0x02, 0x01, 0x02, 0x04}      //^         37
};

static const uint16_t font9x7[][7] = {
    {0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000},      //SPACE         0

    {0x010, 0x038, 0x07C, 0x0FE, 0x1FF, 0x0D6, 0x038},      //PIK           1
    {0x010, 0x038, 0x07C, 0x0FE, 0x07C, 0x038, 0x010},      //KARO          2
    {0x0EE, 0x1FF, 0x1FF, 0x1FF, 0x0FE, 0x07C, 0x010},      //HERC          3
    {0x010, 0x0BA, 0x1FF, 0x1FF, 0x0BA, 0x038, 0x07C},      //TREF          4
    {0x010, 0x054, 0x038, 0x1FF, 0x038, 0x054, 0x010},      //ZVEZDA        5
    {0x0FE, 0x101, 0x100, 0x0FE, 0x001, 0x101, 0x0FE},      //S             6

    {0x07C, 0x082, 0x101, 0x101, 0x101, 0x082, 0x07C},      //PRAZAN KRUG   7
    {0x07C, 0x0E2, 0x1E1, 0x1E1, 0x1E1, 0x0E2, 0x07C},      //POLUPUN KRUG  8
    {0x07C, 0x0FE, 0x1FF, 0x1FF, 0x1FF, 0x0FE, 0x07C},      //PUN KRUG      9
    
    {0x010, 0x028, 0x044, 0x082, 0x101, 0x000, 0x000}      //^             10
};

/* ---------- I2C ---------- */
static void I2C_Init(void) {
    // P3.0 = SDA
    // P3.1 = SCL
    P3SEL |= BIT0 | BIT1;

    UCB0CTL1 |= UCSWRST;

    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;
    UCB0CTL1 = UCSSEL__SMCLK | UCSWRST;
    
    UCB0BR0 = 10;
    UCB0BR1 = 0;

    UCB0I2CSA = SH1106_ADDRESS;

    UCB0CTL1 &= ~UCSWRST;
}

static void I2C_Write(uint8_t control, const uint8_t *data, uint16_t length) {
    int i;
    // Make sure previous transaction is finished
    while (UCB0CTL1 & UCTXSTP);

    // Start transmitting and generate START
    UCB0CTL1 |= UCTR | UCTXSTT;

    // Wait until TX buffer is ready
    while (!(UCB0IFG & UCTXIFG));

    // Send SH1106 control byte
    UCB0TXBUF = control;
    // Send all data bytes
    for (i = 0; i < length; i++) {
        // Wait for previous byte
        while (!(UCB0IFG & UCTXIFG));

        // Send next byte
        UCB0TXBUF = data[i];
    }

    // Make sure final byte is done
    while (!(UCB0IFG & UCTXIFG));

    // Generate STOP
    UCB0CTL1 |= UCTXSTP;

    // Wait for STOP to finish
    while (UCB0CTL1 & UCTXSTP);
}

/* ---------- SH1106 ---------- */
static void OLED_Command(uint8_t command) {
    I2C_Write(0x00, &command, 1);
}

void OLED_Init(void) {
    I2C_Init();

    __delay_cycles(50000);

    OLED_Command(0xAE);    // Display OFF

    OLED_Command(0xD5);    // Display clock
    OLED_Command(0x80);

    OLED_Command(0xA8);    // Multiplex ratio
    OLED_Command(0x3F);

    OLED_Command(0xD3);    // Display offset
    OLED_Command(0x00);

    OLED_Command(0x40);    // Start line

    OLED_Command(0xAD);    // DC-DC control
    OLED_Command(0x8B);

    OLED_Command(0xA1);    // Segment remap

    OLED_Command(0xC8);    // COM scan direction

    OLED_Command(0xDA);    // COM pins
    OLED_Command(0x12);

    OLED_Command(0x81);    // Contrast
    OLED_Command(0x80);

    OLED_Command(0xD9);    // Pre-charge
    OLED_Command(0x1F);

    OLED_Command(0xDB);    // VCOM
    OLED_Command(0x40);

    OLED_Command(0xA4);    // Display follows RAM
    OLED_Command(0xA6);    // Normal display

    OLED_Command(0xAF);    // Display ON

    OLED_Clear();
    OLED_Update();
}

void OLED_Clear(void) {
    uint16_t i;

    for (i = 0; i < sizeof(framebuffer); i++)
    {
        framebuffer[i] = 0;
    }
}

void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t on) {
    uint16_t index;
    uint8_t bit;

    if (x >= SH1106_WIDTH || y >= SH1106_HEIGHT)
        return;

    index = x + (y / 8) * SH1106_WIDTH;
    bit = 1 << (y % 8);

    if (on)
        framebuffer[index] |= bit;
    else
        framebuffer[index] &= ~bit;
}

void OLED_Update(void) {
    uint8_t page;

    for (page = 0; page < 8; page++)
    {
        OLED_Command(0xB0 + page);

        OLED_Command(0x02);
        OLED_Command(0x10);

        I2C_Write(
            0x40,
            &framebuffer[page * 128],
            128
        );
    }
}

void OLED_Test(void) {
    OLED_Clear();

    OLED_Print("SKOCKO BY DJOSTA");

    OLED_Update();
}

void OLED_SetCurs(uint8_t x, uint8_t y) {
    x0 = x;
    y0 = y;
    return;
}

static void OLED_PrintChar(char c) {
    uint8_t column;
    uint8_t row;
    uint8_t index;
    uint8_t data;

    if (c == ' ') {
        x0 += 6;
        return;
    }
    else if (c >= 'A' && c <= 'Z') {
        index = c - 'A' + 1;
    }
    else if (c >= '0' && c <= '9') {
        index = c - '0' + 27;
    }
    else {
        return;
    }

    for (column = 0; column < 5; column++) {
        data = font5x7[index][column];

        for (row = 0; row < 7; row++) {
            if (data & (1 << row))
                OLED_SetPixel(x0 + column, y0 + row, 1);
            else
                OLED_SetPixel(x0 + column, y0 + row, 0);
        }
    }

    x0 += 6;
}

void OLED_Print(const char *text) {
    while (*text) {
        OLED_PrintChar(*text);
        text++;
    }
}

void OLED_PrintSign(uint8_t sign) {
    uint8_t column;
    uint8_t row;
    uint16_t data;

    for (row = 0; row < 7; row++)
    {
        data = font9x7[sign][row];

        for (column = 0; column < 9; column++)
        {
            if (data & (1 << (8 - column)))
                OLED_SetPixel(x0 + column, y0 + row, 1);
            else
                OLED_SetPixel(x0 + column, y0 + row, 0);
        }
    }

    x0 += 10;
}


