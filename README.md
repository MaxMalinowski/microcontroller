# Microcontroller Source Code

This repository contains the solution for the microcontroller class.
The source code can be found in the src directory.
The doc directory contains documentation / a protocoll for the class.

The exercises are implemented step by step and build upon each other.
The main functionality is implemented in the main.c file. This file also contains the declaration of global variables (e.g. for timers) and also the interrupt handlers

This project was developed on the STM32F4 microcontroller discovery board and the Kail MDK-ARM IDE. If installed (windows only), just double click the .uvprojx file in the src directory to open the project.

Because of specialized peripherals, only until exercise 2 the source code is working.
Nevertheless, maybe the code will someone looking for inspiration :D


The exercises solved and their functionallity follows:

## Table of Contents

### Ex. 1

Start: Create and setup a Project using the Keil IDE.


### Ex. 2

A green led is flashing with one Hz when the user butto is pressed.


### Ex. 3

16 external leds can be triggered using a 16bit word. 


### Ex. 4

An external display can be used. Using specialized functions, the output of single letters or complet strings is possible. 
As part of a competition, the display can be cleard in 49.2 milliseconds.


### Ex. 5

From an external 4x4 keyboard matrix, the input can be read.
Using the functions form ex.4, the keyboard status can be printed on the display.

### Ex. 6

Using  timer 7, we derive a 1 millisecond time basis.
Better controll of time dependent tasks is now possible.

### Ex. 7

Using timer 12  an external frequency segnal is measured and displayed. 


### Ex. 8

The microcontroller takes the roll of a LIN slave in a LIN network. The periodically sends out identifier and the programm responses either with the current time, keyboard state or frequency, according to the identifier.


### Ex. 9

The analog signals from two potentiomerts are converted to digital percent values and displayed.


### Ex. 10

With the percent values from ex. 9, it ist now possible to create a pwm signal and controll the position of a throttle.
