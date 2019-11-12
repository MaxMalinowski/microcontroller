#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "keyboard.h"
#include "display.h"
#include "inttypes.h"


void keyboard_Init(void) 
{
	RCC -> AHB1ENR |= 0x00000003;		// Enable Clock for GPIOA and GPIOB
	GPIOB -> MODER &= 0x00000030;   // Clear reset values
	GPIOB -> MODER |= 0x00005500;		// GPIOB 4-7 are outputs
	GPIOB -> OTYPER |= 0x000000F0;	// Set output type to open-drain
	GPIOA -> PUPDR |= 0x00001540;		// Set inputs to pull-up
}


uint16_t keyboard_Read(void)
{
	uint8_t i = 0;
	uint16_t res = 0x0000;

	for(i = 0; i < 4; i++)
	{
		GPIOB -> ODR |= 0x00F0;        	// all outputs (B4-7) high
		GPIOB -> ODR &= ~(0x0010 << i);	// set one port, equals one line low
		res |= (((0x0078 & GPIOA -> IDR) >> 3) << (i * 4)); 	// Check input	
	}	
	return res;
}


char* Short2Bitstring(uint16_t keys)
{
	char array[17];
	char* res;
	uint8_t i = 0;
	
	for(i = 0; i < 16; i++)
	{
		if(((0x0001 << i) & keys) == (0x0001 << i))
		{
			array[i] = '1';	
		}
		else
		{
			array[i] = '0';	
		}	
	}
	array[16] = '\0';
	res = array;
	return res;
}
