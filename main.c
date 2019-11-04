#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"
#include "leds.h"


//-----------------------------------------------------
//MAIN
//-----------------------------------------------------
int main(void) {
	mcpr_SetSystemCoreClock();
	
	LEDs_InitPorts();
	
	while (1) 
	{
		leds_main();
	}
}
