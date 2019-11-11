void LCD_Main(void);

void LCD_Init(void);

void LCD_PortInit(void);

void LCD_Init(void);


void LCD_ClearDisplay(uint16_t color);

void LCD_Delay(int microsec);

void LCD_WriteString(uint32_t x, uint32_t y, uint16_t foreground, uint16_t background, char *string);