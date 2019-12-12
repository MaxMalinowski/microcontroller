#include "inttypes.h"
#include "adc.h"
#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"


void adc_Init(void)
{
    RCC -> AHB1ENR |= 0x00000004;           // turn on clock for GPIOC
    GPIOC -> MODER |= 0x0000000F;           // set PC0, PC1 to analog mode
    RCC -> APB2ENR |= 0x00000100;           // turn on clock for ADC1
    ADC1 -> CR2 |= 0x00000001;              // turn ADC on
    ADC -> CSR |= 0x0000002;              	// enable interrupt
    NVIC_SetPriority(ADC_IRQn, 4);          // set interrupt priority
    NVIC_EnableIRQ(ADC_IRQn);               // enable interrupt
}

void adc_GetPotis(uint16_t* firstPoti, uint16_t* secondPoti)
{
	  ADC1->SQR3 = 0xA;
    ADC1 -> CR2 |= 0x40000000;              // start conversion
    while ((ADC1 -> SR & 0x00000002) != 0x00000002) {}
    *firstPoti = ADC1 -> DR*100/4095;                 // result of first conversion
		ADC1->SQR3 = 0xB;
		ADC1 -> CR2 |= 0x40000000;              // start conversion
    while ((ADC1 -> SR & 0x00000002) != 0x00000002) {}
    *secondPoti = ADC1 -> DR*100/4095;               // result of second conversion

}
