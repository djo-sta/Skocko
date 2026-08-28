**Skocko – MSP430 Embedded Game**
Designed and implemented an embedded guessing game using the **MSP430F5529**, utilizing **ADC** for analog potentiometer input, **GPIO interrupts** for button handling, and **I²C communication** with an external OLED display. Implemented a **state machine**, timer-driven functionality, and custom OLED display control in C.

I have made this as a project for MRS (Microcontroler Computer Systems) class at the university.

## Overview

Skocko is a guessing game where the player attempts to guess a randomly
generated combination of four symbols. The player selects symbols using
a potentiometer and navigates between positions using buttons. The game
provides feedback on each guess through an OLED display.

## Features

- 4-symbol combination guessing game
- Potentiometer-based symbol selection
- Button-based navigation and input confirmation
- OLED graphical interface
- Game state machine
- Interrupt-driven button handling
- ADC-based potentiometer input
- I²C communication with OLED display
- Timer-based functionality

## Game States

The game is implemented using a state machine with three main states:
- `START` – Initializes the game and waits for user input
- `GAME` – Handles symbol selection, guesses, and feedback
- `END` – Displays the result and waits for a restart

## Project Structure

- function.c : 7seg display configuration
- init.c : Hardware initialization
- main.c : Main gameplay loop, state machine and interrupts
- sh1106.c : Driver for OLED display
- skocko.c : Functions and variables used for gameplay flow

## Controls

- **Button 1 (P1.4):** Change selected position
- **Button 2 (P1.5):** Confirm the 4 symbol input
- **Red potentiometer (P6.0):** Change selected symbol
- **Display:** Shows the current and pass guesses as well as results


## Technologies

- C
- MSP430F5529
- Code Composer Studio
- ADC
- GPIO / Interrupts
- Timers
- I²C
- OLED / SH1106
