#include "inttypes.h"

void lcd_PortInit(void);


void lcd_Init(void);


void lcd_ClearDisplay(uint16_t color);


void lcd_WriteString(uint32_t x, uint32_t y, uint16_t foreground, uint16_t background, char *string);

