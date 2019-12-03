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


/*
 * volatile global variables
 * Can change any time (in interrupt)
 * Compiler know through volatile keyword
 */
volatile uint32_t milliSec = 0;         // global milliseconds
volatile uint32_t capt_old = 0;         // old frequency captured
volatile uint32_t capt_new = 0;         // new frequency captured
volatile uint8_t tim12_count = 0;       // count number of interrupts


/*
 * normal global variables
 */
uint32_t mainTime = 0;                  // time for main routine
uint32_t greenTime = 0;                 // time for greed led
uint32_t backgroundTime = 0;            // time for background
uint32_t frequency_Counted = 0;         // frequency calculated by counting
uint32_t frequency_Captured = 0;        // frequency calculated by capturing
uint8_t greenOn = 0;                    // flag if green led is on
uint8_t userOn = 0;                     // flag if green button is pressed
char* keyboard;                         // keyboard state
char* frequency;                        // frequency
char* pwm;                              // mock data
char* lin_data;                         // array, where date for lin communication is stored


/*
 * states for lin communication
 * initial state: wait for lin break
 */
enum lin_state {wait_for_break, wait_for_sync, wait_for_id, send_data};
enum lin_state lin_current = wait_for_break;


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
    if(tim12_count++ > 3)                       // check number of interrupts
    {
        NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);   // if more then 3 interrupts, all values we need
        tim12_count = 0;                        // disable interrupt and set counter to 0 for next time
    }
}

void USART6_IRQn(void)
{
    uint16_t status = USART6 -> SR;                 // save status register
    uint16_t data = USART -> DR;                    // save data register
    USART6 -> SR = 0;                               // delete all flags to be sure

    // state machine
    switch (lin_current)
    {
        case wait_for_break:
            if (status & 0x00000100)                // check is lbd detected
            {
                lin_current = wait_for_sync;        // if lbd detected, wait for synch
            }
            break;
        case wait_for_sync:
            if (status & 0x00000002)                // check if sync break detected
            {
                lin_current = wait_for_id;          // if sync detected, wait for id
            }
            else
            {
                lin_current = wait_for_break        // if not sync, wait for lin break
            }
            break;
        case wait_for_id:
            if (status & 0x00000020)                // check if data received
            {
                switch (data) {                     // check if received data is relevant identifier
                    case 0x18:                      // send temp / pwm
                        lin_current = send_data;
                        lin_PackData(0x18, &pwm, 1, &lin_data);
                        USART6 -> DR = lin_data << 8;
                        break;
                    case 0x28:                      // send frequency
                        lin_current = send_data;
                        lin_PackData(0x28, &frequency, 4, &lin_data);
                        USART6 -> DR = lin_data << 8;
                        break;
                    case 0x38:                      // send keyboard
                        lin_current = send_data;
                        lin_PackData(0x38, &keyboard, 2, &lin_data);
                        USART6 -> DR = lin_data << 8;
                        break;
                    default:                        // identifier not relevant
                        lin_current = wait_for_break;
            }
            else
            {
                lin_current = wait_for_break;
            }
            break;
        case send_data:
            if (status & 0x00000040)                // check if data was send
            {
                if (lin_data[0] != 0x0000)          // check if data to send
                {
                    USART6 -> DR = lin_data << 8;   // if data left, shift into data register
                }
                else
                {
                    lin_current = wait_for_break;   // else wait for lin break
                }
            }
            else
            {
                lin_current = wait_for_break;       // else wait for lin break
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


    /*
     * Local variables
     */
    uint16_t old_keyboard = 0x0000;
    uint16_t new_keyboard = 0x0000;
    char buffer_count[32];
    char buffer_capt[32];

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
        if (frequency_Captured > 1000000)
        {
            frequency = int2Bitstring(&frequency_Captured);
        }
        else
        {
            frequency = int2Bitstring(&frequency_Counted)
        }
        sprintf(buffer_count, "Counter: %8d", frequency_Counted);
        lcd_WriteString(10,10,0x0000,0xFFFF, buffer_count);
        sprintf(buffer_capt, "Capture: %8d", frequency_Captured);
        lcd_WriteString(10,50,0x0000,0xFFFF, buffer_capt);

        /*
         * Reading keyboard input
         * Displaying input on lcd and led's
         */
        old_keyboard = new_keyboard;
        new_keyboard = keyboard_Read();
        led_Write(new_keyboard);
        keyboard_Check(old_keyboard, new_keyboard, keyboard);

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


