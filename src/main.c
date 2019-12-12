#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "stdio.h"
#include "blinky.h"
#include "leds.h"
#include "display.h"
#include "keyboard.h"
#include "timer.h"
#include "frequency.h"
#include "lin.h"
#include "adc.h"
#include "throttle.h"


/*
 * volatile variables
 * Can change any time (in interrupt)
 * Compiler know through volatile keyword
 */
volatile uint32_t milliSec = 0;         // global milliseconds
volatile uint32_t capt_old = 0;         // old frequency captured
volatile uint32_t capt_new = 0;         // new frequency captured
volatile uint8_t tim12_count = 0;       // count number of interrupts


/*
 * normal variables
 */
uint32_t mainTime = 0;                  // time for main routine
uint32_t greenTime = 0;                 // time for greed led
uint32_t backgroundTime = 0;            // time for background
uint32_t frequency_Counted = 0;         // frequency calculated by counting
uint32_t frequency_Captured = 0;        // frequency calculated by capturing
uint16_t old_keyboard = 0;		        // previous keyboard state
uint16_t new_keyboard = 0;		        // current keyboard state
uint16_t firstPoti = 0;                 // value form first poti
uint16_t secondPoti = 0;                // value form second poti
uint8_t greenOn = 0;                    // flag if green led is on
uint8_t userOn = 0;                     // flag if green button is pressed
uint8_t sending_pos = 1;                // current position in lin_data
uint8_t size = 0;                	    // length of lin_data
uint8_t running_cnt = 0;
uint8_t running_st = 0;
char lin_data [5];                      // array, where date for lin communication is stored


/*
 * states for lin communication
 * initial state: wait for lin break
 */
enum lin_state {WAIT_FOR_BREAK, WAIT_FOR_SYNC, WAIT_FOR_ID, SEND_DATA};
enum lin_state lin_mode = WAIT_FOR_BREAK;


/*
 * Interrupts
 */
void TIM7_IRQHandler() {
    TIM7 -> SR = 0;                             // reset status register
    milliSec++;                                 // add one millisecond
}

void TIM8_BRK_TIM12_IRQHandler(void) {
    TIM12 -> SR = 0;                            // reset status register
    capt_old = capt_new;                        // save old frequency value
    capt_new = TIM12 -> CCR1;                   // get new frequency value
    tim12_count++;
    if(tim12_count > 2)                         // check number of interrupts
    {
        NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);   // if more then 3 interrupts, all values we need
    }
}

void USART6_IRQHandler(void)
{
    uint16_t status = USART6 -> SR;                 	// save status register
    uint8_t data = USART6 -> DR;                    	// save data register
    USART6 -> SR = 0;                               	// delete all flags to be sure

    // state machine
    switch (lin_mode)
    {
        case WAIT_FOR_BREAK:
            if (status & 0x100)                     	// check if lbd detected
            {
                lin_mode = WAIT_FOR_SYNC;           	// if lbd detected, wait for sync
            }
            break;

        case WAIT_FOR_SYNC:
            if ((status & 0x20) && (data == 0x55))  	// check if sync break detected
            {
                lin_mode = WAIT_FOR_ID;             	// if sync detected, wait for id
            }
            else
            {
                lin_mode = WAIT_FOR_BREAK;          	// if not sync, wait for lin break
            }
            break;

        case WAIT_FOR_ID:
            if (status & 0x20)                      	// check if data received
            {
				uint32_t tmp = 0;
                switch (data & ~0xC0) 		    		// check if received data is our identifier
                {
                    case 0x18:                      	// send milliseconds
                        lin_mode = SEND_DATA;
												size = 2;
												tmp = milliSec;
                        lin_Send(&tmp, size, 0x18, lin_data);
                        break;

					case 0x28:                      	// send frequency
                        lin_mode = SEND_DATA;
                        size = 4;
                        tmp = frequency_Counted;
                        lin_Send(&tmp, size, 0x28, lin_data);
                        break;

                    case 0x38:                      	// send keyboard
                        lin_mode = SEND_DATA;
						size = 2;
                        tmp = new_keyboard;
                        lin_Send(&tmp, size, 0x38, lin_data);
                        break;

                    default:                        	// identifier not relevant
                        lin_mode = WAIT_FOR_BREAK;
						break;
                }
            }
            else
            {
				lin_mode = WAIT_FOR_BREAK;
            }
            break;

        case SEND_DATA:
            if (status & 0x40)						    // check if data was send
            {
				if (sending_pos <= size)				// check if data left to send
				{
					USART6 -> DR = lin_data[sending_pos++];  	
				}
				else
				{
					lin_mode = WAIT_FOR_BREAK; 
                	sending_pos = 1;
				}
			}
            break;

        default:
            break;
    }
}


/*
 * MAIN-Function
 */
int main(void)
{
    /*
     * Initializations
     */
    mcpr_SetSystemCoreClock();
    led_Init();
    lcd_PortInit();
    lcd_Init();
    lcd_ClearDisplay(0xFFFF);
    keyboard_Init();
    timer7_Init();
    timer12_Init();
    lin_Init();
    adc_Init();
    throttle_Init();


    /*
     * Local variables
     */
    char buffer_count[32];
    char buffer_capt[32];
    char buffer_firstPoti[32];
    char buffer_secondPoti[32];
    

    /*
     * loop forever
     */
    while (1)
    {
        mainTime = milliSec;

        /*
         * Frequency measurements
         */
        timer12_CounterInit();
        timer12_CheckCounter(&milliSec, &frequency_Counted);
        timer12_CaptureInit();
        timer12_CheckCapture(&frequency_Captured, &capt_old, &capt_new, &tim12_count);
        sprintf(buffer_count, "Counter: %8d", frequency_Counted);
        lcd_WriteString(10,10,0x0000,0xFFFF, buffer_count);
        sprintf(buffer_capt, "Capture: %8d", frequency_Captured);
        lcd_WriteString(10,50,0x0000,0xFFFF, buffer_capt);

        /*
         * Reading keyboard input
         * Displaying input on lcd
         */
        old_keyboard = new_keyboard;
        new_keyboard = keyboard_Read();
        keyboard_Check(old_keyboard, new_keyboard);

        /*
         * If key on keyboard pressed, light up led
         * Else display running led light
         */
        if (new_keyboard == 0x0000)
        {
            if (running_st == 0)
            {
                led_Write(1 << running_cnt);
								running_cnt++;
            }
            else
            {
                led_Write(~(1 << (running_cnt)));
								running_cnt++;
            }
            running_cnt = running_cnt % 16;
            if (running_cnt == 0)
            {
                running_st = ~running_st;
            }
        }
        else
        {
            led_Write(new_keyboard);
        }

        /*
         * Read values of Potis
         * Print poti values on lcd display
         */
        adc_GetPotis(&firstPoti, &secondPoti);
        sprintf(buffer_firstPoti, "Poti1: %3d%%", firstPoti);
        lcd_WriteString(10,130,0x0000,0xFFFF, buffer_firstPoti);
        sprintf(buffer_secondPoti, "Poti2: %3d%%", secondPoti);
        lcd_WriteString(10,170,0x0000,0xFFFF, buffer_secondPoti);

        /*
         * control throttle with poti values
         */
        throttle_Control(&firstPoti, &secondPoti);

        /*
         * Checking timer7 conditions
         */
        timer7_CheckLed(&milliSec, &greenTime, &greenOn);
        timer7_CheckBackground(&milliSec, &backgroundTime, &userOn);

        /*
         * Waiting, if loop too fast
         */
        while (milliSec < (mainTime + 50)) {}
    }
}
