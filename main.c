#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "blinky.h"
#include "leds.h"


//-----------------------------------------------------
//MAIN
//-----------------------------------------------------
int main(void) 
{
	mcpr_SetSystemCoreClock();
	
	// Blinky.c init
	// led_port_init();
	
	// leds.c init
	LEDs_InitPorts();
	
	while (1) 
	{
		blinky_main();
		leds_main();
	}
}
