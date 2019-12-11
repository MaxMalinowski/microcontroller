#include "inttypes.h"
#include "adc.h"
#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"


void adc_Init(void)
{
    RCC -> AHB1ENR |= 0x00000004;           // turn on clock for GPIOC
    GPIOC -> MODER |= 0x0000000F;           // set PC0, PC1 to analog mode
    RCC -> APB2ENR |= 0x00000100;           // turn on clock for ADC1
    ADC1 -> CR2 |= 0x00000003;              // turn ADC on, set continuous mode
    ADC1 -> CR1 |= 0x01000000;              // set resolution to 10 bit (not necessary)//now it is 10 bit
    ADC1 -> SQR3 |= 0x0000016A;             // First: ch10 (PC=), Second: ch11 (PC11)
    ADC1 -> SQR1 |= 0x00000000;             // only one conversion //for what?? //not 0x100000?? because of two conversions
    ADC1 -> CR2 |= 0x00000020,              // enable interrupt
    NVIC_SetPriority(ADC_IRQn, 4);          // set interrupt priority
    NVIC_EnableIRQ(ADC_IRQn);               // enable interrupt
}

void adc_GetPotis(uint16_t* firstPoti, uint16_t* secondPoti)
{
    ADC1 -> CR2 |= 0x40000000;              // start conversion
    while ((ADC1 -> SR & 0x00000002) != 0x00000002) {}
    *firstPoti = ADC1 -> DR;                 // result of first conversion
		//ADC1_SR = 0????
    while ((ADC1 -> SR & 0x00000002) != 0x00000002) {}
    *secondPoti = ADC1 -> DR;               // result of second conversion
}
