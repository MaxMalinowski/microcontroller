#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "leds.h"
#include "blinky.h"
#include "math.h"


void led_Init(void)
{
	RCC -> AHB1ENR |= 0x00000018;		// Turn on Clock 
	GPIOD -> MODER |= 0x50554405;		// E and D are outputs
	GPIOE -> MODER |= 0x55554000;
}


void led_Output16BitWord(uint16_t data)
{
	GPIOD -> ODR &= ~0x0000C703;		
	GPIOE -> ODR &= ~0x0000FF80;
	GPIOD -> ODR |= ((data & 0xE000) >> 5);		// Set PD10, PD9, PD8
	GPIOE -> ODR |= ((data & 0x1FF0) << 3);		// Set PE15, PE14, PE13, PE12, PE11, PE10, PE9, PE8, PE7
	GPIOD -> ODR |= ((data & 0x001C) >> 2);		// Set PD1, PD0
	GPIOD -> ODR |= ((data & 0x0003) << 14);	// Set PD15, PD14
}


void led_Write(uint16_t data)
{
	GPIOD -> ODR |= 0x00000820;		// PD5 on, PD11 on
	GPIOD -> ODR &= ~0x000000A0;	// PD7, PD5 off
	led_Output16BitWord(data);		// Send data
	GPIOD -> ODR |= 0x000000A0;		// PD5 on, PD7 on
	GPIOD -> ODR &= ~0x00000800;    // PD11 off
}


void led_Main(void)
{ int i;
	for (i = 0; i < 16; i++)
	{
		led_Write((uint16_t) pow(2, i));
		u_Delay(1000000);
	}
	led_Write(0xFFFF);
	for (i = 0; i < 16; i++)
	{
		led_Write(~(uint16_t) pow(2, i));
		u_Delay(1000000);
	}
}

