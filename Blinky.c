#include "blinky.h"
#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 

void led_port_init(void) 
{
	// Enable clock on gpio port d (bit 3)
	RCC -> AHB1ENR |= 0x00000008;
	// Set green led (port d, bit 12) as output
	GPIOD -> MODER |= 0x01000000;
	// Enable clock on gpio port a (bit 0)
	RCC -> AHB1ENR |= 0x00000001;
	// Set user button (port a, bit 0) as input
	// Not necessary, because reset value is already 0
	// GPIOA -> MODER |= 0x00000000;
}


void u_delay(int microsec) 
{
	int  zaehler =0;
	while(zaehler != microsec * 27)
	{
		zaehler++;
	}
}


void blinky_main(void) 
{
		// Check if user button is pushed
		if (GPIOA -> IDR &0x1)
		{
			// Turn on green led
			GPIOD->ODR |= 0x00001000;
			//Wait 0.5 seconds
			u_delay(500000);
			// Turn of green led
			GPIOD->ODR &= ~0x00010000;
			// Wait 0.5 seconds
			u_delay(500000);
		}
}
