#include "skocko.h"
#include "sh1106.h"
#include <stdlib.h>
#include <time.h>

/*  SINTAKSA ZNAKOVA:                                   SINTAKSA REZULTATA:
    0: nema znaka                                       0: nema rezultata
    1: PIK                                              1: PROMASAJ
    2: KARO                                             2: DOBAR ZNAK, POGRESNO MESTO
    3: HERC                                             3: DOBAR ZNAK, DOBRO MESTO
    4: TREF
    5: ZVEZDA
    6: SKOCKO
*/

uint8_t winning[4] = {0};
volatile uint8_t guess[4] = {0};
volatile uint8_t res[4] = {0};
uint8_t guesses[6][4] = {0};
uint8_t results[6][4] = {0};

volatile uint8_t position = 0;
volatile uint8_t tries = 0;

void MakeWinningCombination() {
    int i;
    srand(ADC12MEM0);

    for (i=0; i<4; i++) winning[i] = rand() % 6 + 1;

    return;
}

void GuessReset() {
    int i;
    for (i=0; i<4; i++) guess[i] = 0;
    position = 0;

    return;
}

static void DecArray() {
    int i, j, max, p;

    for (i=0; i<4; i++) {
        max = -1;
        for (j=i; j<4; j++) {
            if(res[j] > max) {
                p=j;
                max = res[j];
            }
        }

        res[p] = res[i];
        res[i] = max;
    }
}

uint8_t GuessCheck() {
    int tmp[4];
    int i, j, p, b;

    for (i=0; i<4; i++) {
        tmp[i] = winning[i];        //setovanje privremene dobitne kombinacije
        res[i] = 0;                 //resetovanje niza rezultata
    }

    for(i=0; i<4; i++) {
        p=5;
        b=0;
        for (j=0; j<4; j++) {
            if (guess[i] == tmp[j]) {
                if (i == j) {
                    p=j;
                    b=2;
                    break;
                }
                else {
                    if (b == 0) {
                        b = 1;
                        p=j;
                    }
                }
            }
        }

        if (b > 0) {                //dobili smo nekakav pogodak
            tmp[p] = 7;
        }
        res[i] = b;
    }

    DecArray();

    for (i=0; i<4; i++) {
        if (res[i] != 2) return 1;
    }

    return 0;
}

uint8_t NextRow() {
    tries++;
    if (tries == 6) tries = 5;

    return tries;
}

void Saving() {
    uint8_t i;
    for (i=0; i<4; i++) {
        guesses[tries][i] = guess[i];
        results[tries][i] = res[i];
    }
    GuessReset();
    NextRow();
    return;
}

void PrintingResults() {
    int i, j;

    if (tries > 0) {
        for (i=0; i<tries; i++) {
            OLED_SetCurs(1, i*8);
            for (j=0; j<4; j++) OLED_PrintSign(guesses[i][j]);
            OLED_Print("   ");
            for (j=0; j<4; j++) OLED_PrintSign(results[i][j] + 7);
        }
    }
    
    OLED_Update();
}

void Refresh() {
    int j;

    OLED_SetCurs(1, tries*8);
    for (j=0; j<4; j++) OLED_PrintSign(guess[j]);
    OLED_SetCurs(1, (tries+1)*8);
    for (j=0; j<4; j++) OLED_PrintSign(0);
    OLED_SetCurs(1, (tries+1)*8);
    if (position > 0)
        for (j=0; j<position; j++) OLED_PrintSign(0);
    OLED_PrintSign(10);
    OLED_Update();
    return;
}

void Test() {
    tries = 2;
    guesses[0][0] = 1; guesses[0][1] = 3; guesses[0][2] = 1; guesses[0][3] = 2;
    guesses[1][0] = 4; guesses[1][1] = 4; guesses[1][2] = 1; guesses[1][3] = 1;

    results[0][0] = 3; results[0][1] = 3; results[0][2] = 2; results[0][3] = 1;
    results[1][0] = 2; results[1][1] = 2; results[1][2] = 2; results[1][3] = 1;

    PrintingResults();
}

uint8_t NextPosition() {
    position++;
    if (position == 4) position = 0;

    return guess[position];
}

void SignInput(uint8_t sign) {
    if (sign>6) return;
    guess[position] = sign;
}

void PrintCheat() {
    int i;
    OLED_SetCurs(85, 55);
    for (i=0; i<4; i++) OLED_PrintSign(winning[i]);
    Refresh();

    return;
}

void StartScreen() {
    OLED_SetCurs(40, 3);
    OLED_Print("SKOCKO");
    OLED_SetCurs(1, 42);
    OLED_Print("PRITISNUTI DUGME");
    OLED_SetCurs(1, 48);
    OLED_Print("ZA POCETAK");
    OLED_Update();
}

void EndScreen(uint8_t rezultat) {
    OLED_SetCurs(32, 3);
    if (rezultat == 1) {
        OLED_Print("POBEDA :) ");
    }
    else {
        OLED_Print("PORAZ :( ");
    }
    OLED_Update();
    return;
}

