#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"
#include "keyboard.h"
#include "display.h"
#include "timer.h"
#include "frequency.h"
#include "inttypes.h"

volatile uint32_t counterWait=0;

void timer12_Init(void)
{
    RCC -> APB1ENR |= 0x00000040;                // Enable clock for Timer 12
    RCC -> AHB1ENR |= 0x00000002;               // Enable clock for GPIOB
    GPIOB -> MODER &= ~0xC0000000;              // Set Pin 14, 15 to 0
    GPIOB -> MODER |= 0x20000000;               // Set GPIOB to alternate function
    GPIOB -> AFR[1] |= 0x09000000;              // AFR[1] (>Pin 14 in AFRH), 1001 in AFRH14
    TIM12 -> ARR  = 0xFFFF;                     // Set autoreload register to max. value
    TIM12 -> PSC = 0x0000;                      // Set prescaler to  0
    TIM12 -> EGR |= 0x0001;                     // Generate Event
    TIM12 -> CCMR1 |= 0x0001;                   // Select TI1 as input
}


void timer12_CounterInit(void)
{
    TIM12 -> CR1 = 0x0000;                      // Disable counter
    TIM12 -> DIER = 0x0000;                     // Disable interrupt
    TIM12 -> CCER = 0x0000;                     // Disable capture compare
	TIM12 -> SMCR |= 0x0057;                    // Select trigger source (extern source)
	TIM12 -> CR1 = 0x0001;		                // Enable counter
}

void timer12_CaptureInit(void)
{
    TIM12 -> CR1 = 0x0000;                      // Disable counter
    TIM12 -> SMCR = 0x0000;                     // Select trigger source (intern clock)
    TIM12 -> CCER = 0x0001;                     // Enable capture compare (rising)
	NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 1);   // Set interrupt priority
	NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);        // Enable interrupt in nvic
    TIM12 -> DIER |= 0x0002;                    // Enable interrupt
	TIM12 -> CR1 = 0x0001;		                // Enable counter
}


void timer12_CheckCounter(volatile uint32_t* ms, volatile uint32_t* freq_Count)
{
	TIM12 -> CNT = 0;                           // Set counter to 0
	counterWait = *ms;                    // Save current time
	while(*ms < (counterWait + 20)) {}     // wait 20ms
	*freq_Count = TIM12 -> CNT / 0.02;          // calculate frequency
}


void timer12_CheckCapture(volatile uint32_t* freq_Capt, volatile uint32_t* capt_old, volatile uint32_t* capt_new)
{
    *freq_Capt = 0.000011905 * (*capt_new - *capt_old);
}
