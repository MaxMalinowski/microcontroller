#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "tastatur.h"
#include "display.h"
#include "fonts.h"
#include "inttypes.h"


void Tastatur_Init(void) 
{
	RCC -> AHB1ENR |= 0x00000003;		// Turn on Clock for GPIOA and GPIOB 
	//nothing for GPIOA, because it is input
	GPIOB -> MODER &= 0x00000030;   // problem with reset values
	GPIOB -> MODER |= 0x00005500;		// GPIOB 4-7 are outputs
	
	GPIOB -> OTYPER |= 0x000000F0;	// Set output type to open-drain
	

}

void Tastatur_Main(void) {
	uint16_t colorbg = 0xF800; 		// background red
	uint16_t colorfg;							// foreground
	while(1) {
		GPIOD->ODR ^= 0x1000; 			// toggle PD12 (green LED)
		LCD_ClearDisplay(colorbg); 	// clear Display
		LCD_WriteString(220, 220, colorfg, colorbg, "Hallo Welt!");
		colorfg = colorbg;					// Foreground = Background
		colorbg = ~colorbg;					// Backgound = ~Backgound
	}
}
