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
	
	GPIOB -> MODER |= 0x5;		//PB0 and PB1 to output (to control the throttle)
	
	

}

void throttle_Control(uint16_t* firstPoti, uint16_t* secondPoti)
{


}

