#include "inttypes.h"
#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"


void LCD_Init(void);

void LCD_Delay(int microsec);

void LCD_WriteData(uint16_t data);

void LCD_WriteCommand(uint16_t cmd);

void LCD_WriteReg(uint16_t cmd, uint16_t data);

void LCD_WriteLetter(int code);

void LCD_WriteString(char *string);

void LCD_Output16BitWord(uint16_t data);

void LCD_SetCursor(uint32_t x, uint32_t y);

void LCD_DrawPixel(uint16_t color);

void LCD_ClearDisplay(uint16_t color);

void LCD_Main(void);

