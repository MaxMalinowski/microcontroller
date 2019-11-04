#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "leds.h"
#include "math.h"


void LEDs_InitPorts(void)
{
	// Turn on Clock (LEDs and other controlling ports included)
	RCC -> AHB1ENR |= 0x00000018;
	// All leds as outputs
	GPIOD -> MODER |= 0x50150005;
	GPIOE -> MODER |= 0x55554000;
	// All led ports off
	GPIOD -> ODR &= 0x0000C703;
	GPIOE -> ODR &= 0x0000FF80;
}


void LCD_Output16BitWord(uint16_t data)
{
	// Copy of data;  All led ports off
	uint16_t tmp1 = data, tmp2 = data, tmp3 = data, tmp4 = data;
	GPIOD -> ODR &= ~0x0000C703;
	GPIOE -> ODR &= ~0x0000FF80;
	// PD-10,-9,-8
	tmp1 &= 0xE000;
	GPIOD -> ODR |= (tmp1 >> 5);
	// PE-15,-14,-13,-12,-11,-10,-9,-8,-7
	tmp2 <<= 3;
	tmp2 &= 0xFF80;
	GPIOE -> ODR |= tmp2;
	// PD-1,-0
	tmp3 >>= 2;
	tmp3 &= 0x0003;
	GPIOD -> ODR |= tmp3;
	// PD-15,14
	tmp4 &= 0x0003;
	GPIOD -> ODR |= (data << 14);
}


void LEDs_Write(uint16_t data)	
{
	// PD5 on, PD11 on, PD7 off
	GPIOD -> ODR |= 0x00000020;
	GPIOD -> ODR |= 0x00000800;
	GPIOD -> ODR &= ~0x00000080;
	// Send data
	LCD_Output16BitWord(data);
	// PD5 off, PD5 on, PD7 on
	GPIOD -> ODR &= ~0x00000020;
	GPIOD -> ODR |= 0x00000020;
	GPIOD -> ODR |= 0x00000080;
}


void LEDs_delay(int microsec) 
	{
	int  zaehler =0;
	while(zaehler != microsec * 27)
	{
		zaehler++;
	}
}


void leds_main(void)
{
	for (int i = 0; i < 16; i++) 
	{
		LEDs_Write((uint16_t) pow(2, i));
		LEDs_delay(1000000);
	}
	LEDs_Write(0xFFFF);
	for (int i = 0; i < 16; i++) 
	{
		LEDs_Write(~(uint16_t) pow(2, i));
		LEDs_delay(1000000);
	}
}

