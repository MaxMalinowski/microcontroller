#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "keyboard.h"
#include "display.h"
#include "timer.h"
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
	//GPIOA for user button and GPIOD for green led and backlight of display
	RCC 	-> AHB1ENR|= 0x00000009;	// Turn on Clock for GPIOD and GPIOA
	GPIOD -> MODER 	|=0x05000000;		//PD13 is an output and PD12 for green led
	GPIOD -> ODR 		|= 0x2000;			//PD13 is on, PD12 off
	
	
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

void LED(void)
{
	if(greenOn)
	{
		greenTime += milliSec;
	}
	else if((GPIOA -> IDR << 31 & 0xF0000000))
	{
		greenOn = 1;
		GPIOD -> ODR |= 0x00001000;		//green led on
	}
	
	if(greenTime >= 10000)
	{
		greenOn = 0;
		greenTime = 0;
		GPIOD -> ODR &= ~(0x00001000);		//green led out
	}

}

void Background(void)
{	
	if((GPIOA -> IDR << 31 & 0xF0000000))
	{
		greenTime += milliSec;

		if(backgroundOn)
		{
			if(greenTime >=1000)
			{
				backgroundOn = 0;
				GPIOD->ODR &= ~0x00020000;	// Turn off background
				greenTime = 0;
			}
		}
		else
		{
			if(greenTime >=1000)
			{
				backgroundOn = 1;
				GPIOD -> ODR |= 0x00020000; //Turn on background
				greenTime = 0;
			}
		}
			
	}
	else
	{
		backgroundOn = 1;
		GPIOD->ODR |= 0x00002000;  //turn on background
		greenTime = 0;
	}
}

void Reset_Timer_Main(void)
{
	
	//reset the mainTime and milliseconds
	
}





void Timer_Main(void)
{
    Timer_Init();

	while(1)
	{
	    // check times and if time bigger than treshhold, take action
			mainTime = 0;
			LED();
			Background();
				
		while(milliSec<50)
		{
			//do nothing and wait until time is over
		}
		mainTime = 0;
	}
	
}





