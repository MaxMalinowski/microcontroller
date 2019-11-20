#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"
#include "keyboard.h"
#include "display.h"
#include "timer.h"
#include "inttypes.h"


volatile uint32_t milliSec = 0;
volatile uint32_t mainTime = 0;
volatile uint32_t greenTime = 0;
volatile uint32_t backgroundTime = 0;

volatile uint8_t greenOn = 0;
volatile uint8_t userOn = 0;


void Timer_Init(void) 
{
	RCC -> APB1ENR |= 0x00000020;	// enable Timer 7
	TIM7 -> PSC |= 0x00000014;		// set prescaler to 20
	TIM7 -> ARR |= 0x00000F9F;		// set auto-reload register to 3999
	TIM7 -> DIER |= 0x00000001;		// enable interrupt
	TIM7 -> CR1 |= 0x00000001;		// enable counter
}


void TIM7_IRQHandler()
{
    TIM7 -> SR = 0;
    milliSec++;
}


void check_LED(void)
{
	if(GPIOD -> ODR & 0x00001000)                   // check if green led is on (from keyboard)
	{
	    if (!greenOn)                               // Check if led was just turned on
        {
	        greenTime = milliSec;                   // get the current time
	        greenOn = 1;                            // mark led as on
        }

	    if (milliSec > (greenTime++ + 10000))       // Check time and add; if time threshold enter if
        {
	        GPIOD -> ODR &= 0x00001000;             // turn of led
	        greenTime = milliSec;                   // set time to current for further checks
        }
	}
	else                                            // green led is off
    {
	    greenOn = 0;                                // mark led as off
    }
}


void check_Background(void)
{	
	if((GPIOA -> IDR << 31) & 0xF0000000)           // check if user button is pressed
	{
	    if (!userOn)                                // if first 'if' entry set:
	    {
	        backgroundTime = milliSec;              // get time of first press
	        userOn = 1;                             // mark user button as pressed
	    }

	    if (milliSec > (backgroundTime++ + 1000))   // check time and add; if time threshold enter if
        {
            GPIOD->ODR ^= 0x00020000;	            // Toggle background (tun on if off, turn off if on)
            background = milliSec;                  // set time to current for further checks
        }
	}
	else                                            // user pin was not pressed
	{
	    userOn = 0;                                 // set user button as not pressed
		GPIOD->ODR |= 0x00002000;                   // turn on background
	}
}


void Timer_Main(void)
{
    led_port_init();                                // init led and user button
    LCD_PortInit();                                 // init display ports
    LCD_Init();                                     // init display itself
    Keyboard_Init();                                // init keyboard
    Timer_Init();                                   // Timer initialization

    uint16_t old_keys = 0;                          // vars for keyboard (pressed / released / print)
    uint16_t new_keys = 0;
    char keys[17];
    LCD_ClearDisplay(0xFFFF);                       // initial display clear

	while(1)
	{
	    mainTime = milliSec;                        // get time of main-loop start

	    // Keyboard Main stuff
        old_keys = new_keys;                        // set previous key as old key
        new_keys = keyboard_Read();                 // get new key

        Keyboard_Check(old_keys, new_keys, keys);   // check if keys were pressed / released (-> turns on green led)

        check_LED();                                // check if green led is for longer then 10s

        check_Background();                         // check if userbutton is pressed & of toggling of display needed

		while(milliSec < (mainTime + 50)) {}        // wait in case main-loop duration is under 50ms
	}
}
