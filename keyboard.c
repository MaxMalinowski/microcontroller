#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "keyboard.h"
#include "display.h"
#include "blinky.h"
#include "inttypes.h"

volatile uint16_t old_key = 0;
volatile uint16_t new_key = 0;
volatile char keyboard[17];



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
		u_Delay(20);

		res |= (((0x0078 & ~(GPIOA -> IDR)) >> 3) << (i * 4)); 	// Check input	
	
	}	
	return res;
}


void short2Bitstring(uint16_t keys, char* array)
{
	uint16_t i = 0;
	
	for(i = 0; i < 16; i++)
	{
		if(((0x0001 << i) & keys))
		{
			array[15-i] = '1';	
		}
		else
		{
			array[15-i] = '0';	
		}	
	}
	array[16] = '\0';
}

void keyboard_Check(uint16_t old_key, uint16_t new_key, char* keyboard)
{
    short2Bitstring(new_key, keyboard);
    lcd_WriteString(20, 20, 0x0000, 0xFFFF, keyboard);

    if (old_key != new_key)
    {
        if (~old_key & (old_key ^ new_key))
        {
            if(0x00FF & new_key)
            {
                GPIOD->ODR ^= 0x1000;
            }
        }
        else
        {
            if(0xFF00 & old_key)
            {
                GPIOD->ODR ^= 0x1000;
            }
        }
    }
}

void keyboard_Main(void)
{
		old_key = new_key;
		new_key = keyboard_Read();

        short2Bitstring(new_key, keyboard);
        lcd_WriteString(20, 20, 0x0000, 0xFFFF, keyboard);

        if (old_key != new_key)
        {
            if (~old_key & (old_key ^ new_key))
            {
                if(0x00FF & new_key)
                {
                    GPIOD->ODR ^= 0x1000;
                }
            }
            else
            {
                if(0xFF00 & old_key)
                {
                    GPIOD->ODR ^= 0x1000;
                }
            }
        }
}





