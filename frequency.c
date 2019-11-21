#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"
#include "keyboard.h"
#include "display.h"
#include "timer.h"
#include "frequency.h"
#include "inttypes.h"


volatile uint32_t capture_old = 0;
volatile uint32_t capture_new = 0;



void timer12_CountInit(void)
{	
	//GPIOB->MODER |= 0x20000000;	//Alternativ-function for PB14
	RCC -> APB1ENR |= 0x00000040;	        // enable Timer 12
	TIM12 -> ARR = 0xFFFF;		        // set auto-reload register to 3999
	TIM12->CCMR1 |= 0x0001;
	TIM12->CCER |= 0x0001;		// mit &
	TIM12->SMCR |= 0x57;
	TIM12 -> CR1 |= 0x00000001;		        // enable counter
}

void timer12_CaptureInit(void)
{	
	//GPIOB is on	
	//GPIOB->MODER |= 0x20000000;	//Alternativ-function for PB14
	//GPIOB->AFR |= 

	RCC -> APB1ENR |= 0x00000040;	        // enable Timer 12
	TIM12 -> ARR = 0xFFFF;		        // set auto-reload register to 3999
	TIM12->CCMR1 |= 0x0001;
	TIM12->CCER |= 0x0001;
	TIM12 -> DIER |= 0x00000002;		        // enable interrupt if overflow
	NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 1);   // set interrupt priority
	NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);        // enable interrupt in nvic
	TIM12 -> CR1 |= 0x00000001;		        // enable counter
}


void timer12_CheckCounter(void)
{
	/*TIM12 -> CNT = 0;
	counterWait = milliSec;
	while(milliSec < (counterWait + 20)) {}
	counterHz = 1/20/1000/(TIM12->CNT-1);*/
	
}


void timer12_CheckCapture(void)
{
   // captureHz = 1/(capture_new - capture_old)/84000000;
	
}
