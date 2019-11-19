#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "keyboard.h"
#include "display.h"
#include "inttypes.h"


volatile uint32_t milliSec = 0;
volatile uint32_t mainTime = 0;
volatile uint32_t greenTime = 0;
volatile uint32_t backgroundTime = 0;

volatile uint8_t greenOn = 0;
volatile uint8_t backgroundOn = 0;
volatile uint8_t UserOn = 0;


void Timer_Init(void) 
{
	RCC -> APB1ENR |= 0x00000020;	// enable Timer 7
	TIM7 -> PSC |= 0x00000014;		// set prescaler to 20
	TIM7 -> ARR |= 0x00000F9F;		// set auto-reload register to 3999
	TIM7 -> DIER |=0x00000001;		// enable interrupt
	TIM7 -> CR1 |=0x00000001;		// enable counter
}

void TIM7_IRQHandler()
{
    TIM7 -> SR = 0;
    milliSec++;
}

void Timer_Main(void)
{
    Timer_Init();

	while(1)
	{
	    // check times and if time bigger than treshhold, take action
	

	}
	
}





