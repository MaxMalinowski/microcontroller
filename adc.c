#include "inttypes.h"
#include "adc.h"


void adc_Init(void)
{
    RCC -> AHB1 |= 0x00000004;              // turn on clock for GPIOC
    GPIOC -> MODER |= 0x0000000F;           // set PC0, PC1 to analog mode
    RCC -> APB2ENR |= 0x00000100;           // turn on clock for ADC1
    ADC -> CR2 |= 0x00000003;               // turn ADC on, set continuous mode
    ADC -> CR1 |= 0x02000000;               // set resolution to 10 bit (not necessary)
    ADC -> SQR3 |= 0x0000016A;              // First: ch10 (PC=), Second: ch11 (PC11)
    ADC -> SQR1 |= 0x00000000;              // only one conversion
    ADC -> CR2 |= 0x00000020,               // enable interrupt
    NVIC_SetPriority(ADC_IRQn, 4);          // set interrupt priority
    NVIC_EnableIRQ(ADC_IRQn);               // enable interrupt
}

void adc_GetPotis(uint16_t* firstPoti, uint16_t* secondPoti)
{
    ADC -> CR2 |= 0x40000000;               // start conversion
    while ((ADC -> SR & 0x00000002) != 0x00000002) {}
    *FirstPoti = ADC -> DR;                 // result of first conversion
    while ((ADC -> SR & 0x00000002) != 0x00000002) {}
    *SecondPoti = ADC -> DR;                // result of second conversion
}