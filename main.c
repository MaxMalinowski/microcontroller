#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
//#include "Blinky.h"
#include "display.h"
#include "tastatur.h"

//main

int main(void) 
{
	mcpr_SetSystemCoreClock();
	LCD_PortInit();
	LCD_Init();
	Tastatur_Init();
	
	while(1)
	{
		
		//LCD_Main();
		Tastatur_Main();
		
	}
}
