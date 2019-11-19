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
	// Init keyboard, display, led, user button
	led_port_init();
	LCD_PortInit();
	LCD_Init();
	Keyboard_Init();

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
	if(GPIOD -> ODR & 0x00001000)
	{
	    if (!greenOn)
        {
	        greenTime = milliSec;
	        greenOn = 1;
        }

	    if (milliSec > (greenTime++ + 10000))
        {
	        GPIOD -> ODR &= 0x00001000;
	        greenOn = 0;
        }
	}
}


void check_Background(void)
{	
	if((GPIOA -> IDR << 31) & 0xF0000000)
	{
	    if (!userOn)
	    {
	        backgroundTime = milliSec;  // If user button just pressed get time
	        userOn = 1;                 // To no reset press-time
	    }

	    if (milliSec > (backgroundTime++ + 1000))   // check time and add
        {
            GPIOD->ODR ^= 0x00020000;	// Toggle background
            background = milliSec;      // reset time to new
        }
	}
	else
	{
	    userOn = 0;
		GPIOD->ODR |= 0x00002000;       //turn on background
	}
}


void Timer_Main(void)
{
    Timer_Init();

    // Keyboard variables
    uint16_t old_key = 0;
    uint16_t new_key = 0;
    char keyboard[17];
    LCD_ClearDisplay(0xFFFF);

	while(1)
	{
	    mainTime = milliSec;

	    // Keyboard Main stuff
        old_key = new_key;
        new_key = keyboard_Read();

        Short2Bitstring(new_key, keyboard);
        LCD_WriteString(20, 20, 0x0000, 0xFFFF, keyboard);

        if (old_key != new_key)
        {
            if (~old_key & (old_key ^ new_key))
            {
                if(0x00FF & new_key)
                {
                    GPIOD->ODR ^= 0x1000;
                }
            }
            else
            {
                if(0xFF00 & old_key)
                {
                    GPIOD->ODR ^= 0x1000;
                }
            }
        }

	    // Check if green led is on
	        // If on, check how long
	        // > 10s, turn of
        check_LED();

	    // Check if user button is pressed
            // If button pressed, check time > 1s
            // Toggle, if over threshold
        check_Background();

        // Wait, if duration of main loop < 50ms
		while(milliSec < (mainTime + 50))
		{
			//do nothing and wait until time is over
		}
	}
}





