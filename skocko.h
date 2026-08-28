#ifndef SKOCKO_H_
#define SKOCKO_H_
#include "msp430.h"
#include "stdint.h"

void MakeWinningCombination();
uint8_t GuessCheck();
void GuessReset();
void Input();
void Saving();
void PrintingResults();
void Test();
void StartScreen();
void EndScreen(uint8_t rezultat);
uint8_t NextPosition();
void SignInput(uint8_t sign);
void Refresh();
void PrintCheat();

#endif
