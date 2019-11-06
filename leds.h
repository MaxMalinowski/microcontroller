// All initialization action for the leds
void LEDs_InitPorts(void);

// Process 16 bit word 
void LED_Output16BitWord(uint16_t data);

// Write 16 bit word on data bus
void LEDs_Write(uint16_t data);

// LEDs main funcion
void leds_main(void);

// Delay function from Blinky
void LEDs_delay(int microsec);
