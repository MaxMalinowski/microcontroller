#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h" 
#include "display.h"
#include "fonts.h"
#include "inttypes.h"


void lcd_Delay(int microsec)
{
	int  zaehler =0;
	while(zaehler != microsec * 27)
	{
		zaehler++;
	}
}


void lcd_Reset(void)
{
	LCD_Delay(15);
	GPIOD -> ODR |= 0x00000008;
}


void lcd_PortInit(void)
{
	RCC -> AHB1ENR |= 0x0000001A;		// Turn on Clock 
	GPIOB -> MODER |= 0x00050000;		// B, D, E are outputs
	GPIOD -> MODER |= 0x55554505;
	GPIOE -> MODER |= 0x55554040;
	GPIOD -> ODR |= 0x000020B0; 		//PD13, PD4, PD7, PD5 on
}

void lcd_Output16BitWord(uint16_t data)
{
	GPIOD -> ODR &= ~0x0000C703;		
	GPIOE -> ODR &= ~0x0000FF80;
	GPIOD -> ODR |= ((data & 0xE000) >> 5);		// Set PD10, PD9, PD8
	GPIOE -> ODR |= ((data & 0x1FF0) << 3);		// Set PE15, PE14, PE13, PE12, PE11, PE10, PE9, PE8, PE7
	GPIOD -> ODR |= ((data & 0x001C) >> 2);		// Set PD1, PD0
	GPIOD -> ODR |= ((data & 0x0003) << 14);	// Set PD15, PD14
}	


void lcd_WriteData(uint16_t data)
{
	GPIOE -> ODR |= 0x00000008;   // PE3 high
	GPIOD -> ODR &= ~0x00000080;	// PD7 low
	LCD_Output16BitWord(data);		// write
	GPIOD -> ODR &= ~0x00000020;  // PD5 low
	GPIOD -> ODR |= 0x000000A0;		// PD5, PD7 high
}


void lcd_WriteCommand(uint16_t cmd)
{	
	GPIOE -> ODR &= ~0x00000008;	// PE3 low
	GPIOD -> ODR &= ~0x00000080;	// PD7 low
	LCD_Output16BitWord(cmd);			// write
	GPIOD -> ODR &= ~0x00000020;	// PD5 low
	GPIOD -> ODR |= 0x000000A0;		// PD5, PD7 high
}


void lcd_WriteReg(uint16_t cmd, uint16_t data)
{
	LCD_WriteCommand(cmd);	// Write command
	LCD_WriteData(data);		// Write data
}


void lcd_Init(void)
{
	LCD_Reset();
	LCD_WriteReg(0x0010, 0x0001);		/* Enter sleep mode */ 
	LCD_WriteReg(0x001E, 0x00B2);		/* Set initial power parameters. */ 
	LCD_WriteReg(0x0028, 0x0006);		/* Set initial power parameters. */
	LCD_WriteReg(0x0000, 0x0001);		/* Start the oscillator.*/ 
	LCD_WriteReg(0x0001, 0x72EF); 	/* Set pixel format and basic display orientation */ 
	LCD_WriteReg(0x0002, 0x0600); 
	LCD_WriteReg(0x0010, 0x0000);		/* Exit sleep mode. 30ms warten weniger geht meist auch */
	LCD_Delay(30000);
	LCD_WriteReg(0x0011, 0x6870); 	/* Configure pixel color format and MCU interface parameters.*/ 
	LCD_WriteReg(0x0012, 0x0999);		/* Set analog parameters */ 
	LCD_WriteReg(0x0026, 0x3800); 
	LCD_WriteReg(0x0007, 0x0033);		/* Enable the display */ 
	LCD_WriteReg(0x000C, 0x0005);		/* Set VCIX2 voltage to 6.1V.*/ 
	LCD_WriteReg(0x000D, 0x000A);		/* Configure Vlcd63 and VCOMl */ 
	LCD_WriteReg(0x000E, 0x2E00); 
	LCD_WriteReg(0x0044, (240-1) << 8); /* Set the display size and ensure that the GRAM window is set to allow access to the full display buffer.*/ 
	LCD_WriteReg(0x0045, 0x0000); 
	LCD_WriteReg(0x0046, 320-1); 
	LCD_WriteReg(0x004E, 0x0000); 		/*Set cursor to 0,0 */ 
	LCD_WriteReg(0x004F, 0x0000); 
}


void lcd_SetCursor(uint32_t x, uint32_t y)
{
	LCD_WriteReg(0x004E, x);
	LCD_WriteReg(0x004F, y);	
}


void lcd_DrawPixel(uint16_t color)
{
	LCD_WriteReg(0x0022, color);	
}


void lcd_ClearDisplay(uint16_t color)
{
	uint32_t pixel = 76800;		// number of pixels to color
	LCD_WriteCommand(0x0022);	// set command once
	while (pixel--)
	{
		LCD_WriteData(color);		// command set, just color
	}
}


void lcd_WriteLetter(int letter, int x, int y, uint16_t foreground, uint16_t background)
{
	uint32_t start = letter * 32;												// Start in fonts.h		
	uint32_t end = start + 32;													// End in fonts.h
	uint8_t i = 0;
	while (start < end)
	{
		LCD_SetCursor(x, y++);														// Add line break
		unsigned char val = console_font_12x16[start++];	// Get first byte
		for(i = 0; i < 12; i++) 													// Print first 12 Bit
		{
			if(i == 8) {
				val = console_font_12x16[start++];						// Fist byte printed, get second byte
			}
			if (((0x80 >> (i % 8)) & val) == 0)										// If bit not set, it's background
			{
				LCD_DrawPixel(background);										// Color in background
			}
			else																						// Else bit is set, it's foreground
			{
				LCD_DrawPixel(foreground);										// Color in background
			}
		}
	}
}


void lcd_WriteString(uint32_t x, uint32_t y, uint16_t foreground, uint16_t background, char *string)
{
	uint16_t i = 0;
	do 
	{
		LCD_WriteLetter((int) string[i], x, y, foreground, background);
		x += 12;
	} 
	while (string[++i] != '\0');
}


void lcd_Main() {
	LCD_PortInit();
	LCD_Init();
	
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
