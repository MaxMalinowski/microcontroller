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
	TIM7 -> ARR = 0x00000F9F;		        	// set auto-reload register to 3999
	TIM7 -> DIER |= 0x00000001;		        // enable interrupt if overflow
	NVIC_SetPriority(TIM7_IRQn, 5);       // set interrupt priority
	NVIC_EnableIRQ(TIM7_IRQn);            // enable interrupt in nvic
	TIM7 -> CR1 |= 0x00000001;		        // enable counter
}


void timer7_CheckLed(volatile uint32_t* ms, uint32_t* gt, uint8_t* go)
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


void timer7_CheckBackground(volatile uint32_t* ms, uint32_t* bt, uint8_t* uo)
{	
	if(GPIOA -> IDR & 0x1)                  // check if user button is pressed
	{
	    if (!uo)                            // if first 'if' entry set:
	    {
	        *bt = *ms;                      // get time of first press
	        *uo = 1;                        // mark user button as pressed
	    }

	    if (*ms > (*bt + 1000))             // check time and add; if time threshold enter if
        {
            GPIOD->ODR ^= 0x2000;	        // Toggle background (tun on if off, turn off if on)
            *bt = *ms;                    // set time to current for further checks
        }
	}
	else                                    // user pin was not pressed
	{
	    *uo = 0;                            // set user button as not pressed
		GPIOD->ODR |= 0x2000;               	// turn on background
	}
}
