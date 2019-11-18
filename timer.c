#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "keyboard.h"
#include "display.h"
#include "inttypes.h"


void Timer_Init(void) 
{
	//init GPIO
	
	
	
	
	
	RCC -> APB1ENR |= 0x00000020;	//Enable Timer 7
	TIM7-> PSC |= 0x00000014;			//prescaler to 20
	TIM7 -> ARR |= 0x00000FA0;		// auto-reload register to 4000
	
	
	TIM7->DIER |=0x00000001;		//ENABLE interrupt
	TIM7->CR1 |=0x00000001;		//enable counter
	
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





