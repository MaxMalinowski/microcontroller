#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"
#include "keyboard.h"
#include "display.h"
#include "timer.h"
#include "frequency.h"
#include "inttypes.h"


uint32_t counterWait = 0;
uint16_t difference = 0;


void timer12_Init(void)
{
	
    RCC -> APB1ENR |= 0x00000040;               // Enable clock for Timer 12
    RCC -> AHB1ENR |= 0x00000002;               // Enable clock for GPIOB
    GPIOB -> MODER |= 0x20000000;               // Set GPIOB PIN 14 to alternate function
    GPIOB -> AFR[1] |= 0x09000000;              // AFR[1] (>Pin 14 in AFRH), 1001 in AFRH14
    TIM12 -> ARR = 0xFFFF;                      // Set autoreload register to max. value
    TIM12 -> PSC = 0x0000;                      // Set prescaler to  0
    TIM12 -> EGR |= 0x0001;                     // Generate Event
    TIM12 -> CCMR1 |= 0x0001;                   // Select TI1 as input
}


void timer12_CounterInit(void)
{
    TIM12 -> DIER = 0x0000;                     // Disable interrupt
    TIM12 -> CCER = 0x0000;                     // Disable capture compare
    TIM12 -> SMCR |= 0x0057;                    // Select trigger source (extern source)
    TIM12 -> CR1 = 0x0001;		        // Enable counter
}


void timer12_CaptureInit(void)
{
    TIM12 -> SMCR = 0x0000;                     // Select trigger source (intern clock)
    TIM12 -> CCER = 0x0001;                     // Enable capture compare (rising)
    NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 1);   // Set interrupt priority
    NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);        // Enable interrupt in nvic
    TIM12 -> DIER |= 0x0002;                    // Enable interrupt
    TIM12 -> CR1 = 0x0001;		        // Enable counter
}


void timer12_CheckCounter(volatile uint32_t* ms, uint32_t* freq_Count)
{	
    counterWait = *ms;                    				// Save current time
    while(*ms < (counterWait + 1)) {}     				// wait 1ms (one interrupt)
    TIM12 -> CNT = 0;                      				// Set counter to 0
    while(*ms < (counterWait + 11)) {}     				// wait 10ms
    *freq_Count = (TIM12 -> CNT) * 100;  				// calculate frequency (*100 = /0.01)
}


void timer12_CheckCapture(uint32_t* freq_Capt, volatile uint32_t* capt_old, volatile uint32_t* capt_new, volatile uint8_t* tim12_count)
{
    while (*tim12_count <= 3) {}					// wait for 3 interrups, to have good values 
    difference = (uint16_t)*capt_new - (uint16_t)*capt_old;		// calculate difference -> casting needed to prevent nasty things
    *freq_Capt = 84000000 / difference;					// save frequncy
}



void int2Bitstring(uint32_t keys, char* array)
{
    uint16_t i = 0;

    for(i = 0; i < 32; i++)
    {
        if(((0x00000001 << i) & keys))
        {
            array[32-i] = '1';
        }
        else
        {
            array[32-i] = '0';
        }
    }
    array[32] = '\0';
}

