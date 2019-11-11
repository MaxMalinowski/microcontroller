#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "tastatur.h"
#include "display.h"
//#include "fonts.h"
#include "inttypes.h"


void Tastatur_Init(void) 
{
	RCC -> AHB1ENR |= 0x00000003;		// Turn on Clock for GPIOA and GPIOB 
	//nothing for GPIOA, because it is input
	GPIOB -> MODER &= 0x00000030;   // problem with reset values
	GPIOB -> MODER |= 0x00005500;		// GPIOB 4-7 are outputs
	
	GPIOB -> OTYPER |= 0x000000F0;	// Set output type to open-drain
	GPIOA -> PUPDR |= 0x00001540;		//Set inputs to pull-up

}

uint16_t Tastatur_Read(void)
{
	int i=0;
	int x =0;
	int start =0;
	uint16_t res =0;
	for(i=0; i<4;i++){
		GPIOB -> ODR = ~0x1<<(i+4);	 //low at one Port
		GPIOB -> ODR &= 0xF0;  //set all other pins to 0
		
		start = i*4;
		for(x=0; x<4;x++)
		{
				res = GPIOA -> IDR ;  //ich bin müde und blick nimma durch.
			//hier soll der ganze kack so verschoben werden, dass mit bitoperationen
			//alles direkt in die variable geschoben wird. 
			//vielleicht kapier ichs ja morgen 
			
			
		}
		
		
		
	}
	
	
	
	return res;
}

/*char[] Short2Bitstring(uint16_t tasten)
{
	
	return 0;
}*/

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
