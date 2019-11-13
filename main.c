#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"
#include "leds.h"
#include "display.h"
#include "keyboard.h"

//main

int main(void) 
{
	mcpr_SetSystemCoreClock();
	
	
	LCD_Main();
	//Keyboard_Main();
	
}
