#ifndef SH1106_H
#define SH1106_H

#include <stdint.h>

#define SH1106_WIDTH    128
#define SH1106_HEIGHT    64
#define SH1106_ADDRESS  0x3C

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Update(void);

void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t color);
void OLED_SetCurs(uint8_t x, uint8_t y);
void OLED_Print(const char* text);
void OLED_PrintSign(uint8_t sign);

void OLED_Test(void);

#endif
