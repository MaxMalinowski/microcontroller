#include "throttle.h"
#include "inttypes.h"
#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"


void throttle_Init(void)
{
	RCC -> AHB1ENR |=0xF;	//ENABLE PB, PA, PD, PC 
	GPIOD -> MODER |= 0x1000; //PD 6 to output(PD6 is status flag)
	GPIOD -> OTYPER |= 0x40;	//PD6 output open drain
	GPIOD -> PUPDR |= 0x1000; 	//PD6 pull up
	
	GPIOC -> MODER |= 0x400000;	//PC11 to output(PC11 is enable)
	GPIOC -> ODR |= (1<<11);
	
	GPIOB -> MODER |= 0xA;		//PB0 and PB1 to alternate function (to control the throttle)
	GPIOB -> AFR[0] |= 0x22;
	
	RCC -> APB1ENR |= 0x00000002;               // Enable clock for Timer 3
	TIM3 -> CR1 |= 0x1;		//count mit prescaler verbinden
	TIM3 -> CCMR2 |= 0x6060;                   // config PWM
	TIM3 -> CCER |= 0x1100;
	TIM3 -> ARR = 99;     	// Set autoreload register to max. value
	TIM3 -> PSC = 839;
	
	

}

void throttle_Control(uint16_t* firstPoti, uint16_t* secondPoti)
{
	TIM3 -> CCR3 = *firstPoti;
	TIM3 -> CCR4 = *secondPoti;
}

