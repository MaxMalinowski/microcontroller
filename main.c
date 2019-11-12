#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"
#include "leds.h"
#include "display.h"
#include "keyboard.h"

//main

int main(void) 
{
	mcpr_SetSystemCoreClock();
	LCD_PortInit();
	LCD_Init();
	keyboard_Init();
	
	uint16_t old_key = 0;
	uint16_t new_key = 0;
	uint8_t pressed = 0x00;
	uint8_t released = 0x00;
	
	while(1)
	{
		old_key = new_key;
		new_key = keyboard_Read();
		
		char* live = Short2Bitstring(new_key);
		LCD_WriteString(20, 20, 0x0000, 0xFFFF, live);

		if ((old_key != new_key) && (~old_key & (old_key ^ new_key))) // Key was pressed
		{
			pressed ^= pressed;
			LEDs_Write(pressed << 8);
		}
		if ((old_key != new_key) && (old_key & ~(old_key ^ new_key)))	// a key was released
		{
			released ^= released;
			LEDs_Write(released);
		}
	}
}
