# Skocko – MSP430 Embedded Game

I designed and implemented an embedded guessing game using the **MSP430F5529**, utilizing **ADC** for analog potentiometer input, **GPIO interrupts** for button handling, and **I²C communication** with an external OLED display. Implemented a **state machine**, timer-driven functionality, and custom OLED display control in C.

I have made this as a project for MRS (Microcontroler Computer Systems) class at the university.

## Overview

Skocko is a guessing game where the player attempts to guess a randomly
generated combination of four symbols. The player selects symbols of their guess,
submits a guess and then receives a result which is presented as 4 circles that
can be:
  1. full (you guessed the right sign at the right spot)
  2. half-full (you guessed the right sign, but missed its spot)
  3. empty (you missed the sign)

Circles do not correspond to signs in their place (1st circle doesn't represent
the result of a 1st sign).

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
- `START` – Initializes the game and waits for user input to start the game
- `GAME` – Handles symbol selection, guesses, and feedback
- `END` – Displays the result and waits for user input to restart the game

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

## Signs and Potentiometer

There are 6 signs int the Skocko game: Spades, Diamonds, Hearts, Clubs,
Star and Skocko, and there is also a reserved spot for Empty sign (no sign
selected yet). I used ADC with a range 0-4095 to differentiate these 7 options
evenly, so starting with Empty, i split 4096 different values into 7 equal
zones, each assigned its own sign. When moving spots, the zone that you're currently 
in is always assigned to the sign that was previously at that spot, and the other 
zones are moved so the same arrangement is always kept. 

## ISR

Program utilizes two maskable interrupts that are used mostly used during
the combination selection. First one is representing the press of buttons on 
P1.4 and P1.5, and the other one for calculating ADC value every time it changes.
When either button is pressed during START and END an interrupt changes a
button_flag which signals the main loop to enter GAME state.
During the game:
- both button presses and potentiometer change trigger an interrupt_flag which
tells the main loop that a change happened and screen needs refreshing;
- button P1.4 also tells that position change happened and recalibrates the ADC
zone placement;
- button P1.5 also tells that the guess is finished and that it should be
processed;
- when potentiometer position is moved, the interrupt tells ADC to calculate
new value.

## Display and I²C

Display that i used for this project was a cheap one bought from TEMU that has
sh1106 chip insed it, so before connecting it to the MSP I needed to make
custom drivers for it. Protocol that I decided to use is I²C since its the 
easiest one to configure and I already used it in the past. Drivers used here
were just a modification of the ones I already made to connect the same display 
to my ESP32 and are placed in sh1106.c file.

## Assembly

There was a specific task: a project needed a piece of its code to be written in
Assembly, so I decided that it should be a part that compares my guess to the winning
combination and named it GuessCheckAsm. The assembly code was made to imitate the part 
of the code commented out in ckocko.c file, GuessCheck() function part.


No functional code was written by **AI**, only time it was used is for making sign bitmaps
(and even that I needed to fix by hand unfortunatly).
