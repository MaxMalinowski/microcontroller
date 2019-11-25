#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"
#include "keyboard.h"
#include "display.h"
#include "timer.h"
#include "inttypes.h"


void timer7_Init(void)
{
	RCC -> APB1ENR |= 0x00000020;	        // enable Timer 7
	TIM7 -> PSC |= 0x00000014;		        // set prescaler to 20
	TIM7 -> ARR = 0x00000F9F;		        // set auto-reload register to 3999
	TIM7 -> DIER |= 0x00000001;		        // enable interrupt if overflow
	NVIC_SetPriority(TIM7_IRQn, 0);         // set interrupt priority
	NVIC_EnableIRQ(TIM7_IRQn);              // enable interrupt in nvic
	TIM7 -> CR1 |= 0x00000001;		        // enable counter
}


void timer7_CheckLed(volatile uint32_t* ms, volatile uint32_t* gt, volatile uint8_t* go)
{
	if(GPIOD -> ODR & 0x00001000)           // check if green led is on (from keyboard)
	{
	    if (!*go)                           // Check if led was just turned on
        {
	        *gt = *ms;                      // get the current time
	        *go = 1;                        // mark led as on
        }

	    if (*ms > (*gt + 10000))            // Check time and add; if time threshold enter if
        {
	        GPIOD -> ODR &= ~0x00001000;    // turn of led
	        *gt = *ms;                      // set time to current for further checks
        }
	}
	else                                    // green led is off
    {
	    *gt = 0;                            // mark led as off
    }
}


void timer7_CheckBackground(volatile uint32_t* ms, volatile uint32_t* bt, volatile uint8_t* uo)
{	
	if(GPIOA -> IDR & 0x1)                  // check if user button is pressed
	{
	    if (!uo)                           // if first 'if' entry set:
	    {
	        *bt = *ms;                      // get time of first press
	        *uo = 1;                        // mark user button as pressed
	    }

	    if (&ms > (&bt + 1000))             // check time and add; if time threshold enter if
        {
            GPIOD->ODR ^= 0x2000;	        // Toggle background (tun on if off, turn off if on)
            *bt = *ms;                      // set time to current for further checks
        }
	}
	else                                    // user pin was not pressed
	{
	    *uo = 0;                             // set user button as not pressed
		GPIOD->ODR |= 0x2000;               // turn on background
	}
}


/*void timer7_Main(void)
{
    led_port_init();                                // init led and user button
    LCD_PortInit();                                 // init display ports
    LCD_Init();                                     // init display itself
    keyboard_Init();                                // init keyboard
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
}*/
