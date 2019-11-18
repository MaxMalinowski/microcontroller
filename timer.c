#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "keyboard.h"
#include "display.h"
#include "inttypes.h"


void Timer_Init(void) 
{
	/*RCC -> AHB1ENR |= 0x00000003;		// Enable Clock for GPIOA and GPIOB
	GPIOB -> MODER &= 0x00000030;   // Clear reset values
	GPIOB -> MODER |= 0x00005500;		// GPIOB 4-7 are outputs
	GPIOB -> OTYPER |= 0x000000F0;	// Set output type to open-drain
	GPIOA -> PUPDR |= 0x00001540;		// Set inputs to pull-up*/
	
	//Timer inits
}

void Timer_Main(void)
{
	//Inits

	while(1)
	{
	
		//Dauerschleife
	}
	
}





