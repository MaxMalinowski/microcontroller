#include "lin.h"
#include "inttypes.h"
#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"


void lin_Init(void)
{
    RCC -> AHB1ENR |= 0x00000004;               // enable clock for GPIOC
		GPIOB -> MODER |= 0x00000010;
    GPIOC -> MODER |= 0x00006000;               // set Pin 6 to alternate and 7 to output
	
		GPIOC -> ODR  |= (1<<7);
		GPIOB -> ODR |= 0x4;
	
		GPIOC -> MODER = 0x0000A000;		
	
    GPIOC -> AFR[0] |= 0x88000000;              // select alternate functions 
    RCC -> APB2ENR |= 0x00000020;               // enable clock for usart6
    USART6 -> BRR = 0x00001117;                 // set baudrate 273,4375
    USART6 -> CR1 |= 0x0000206C;                // enable RE, TE, RXNEIE, TCIE, UE
    USART6 -> CR2 |= 0x00004040;                // enable LINEN, LBDIE
    NVIC_SetPriority(USART6_IRQn, 0);           // set interrupt priority
    NVIC_EnableIRQ(USART6_IRQn);                // enable interrupt
}


uint8_t lin_Checksum(uint8_t* dataptr, uint8_t size, uint8_t id)
{
    uint8_t i = 0;
    uint8_t res = 0;
    uint16_t tmp = id;

    for(i = 0; i < size; i++)
    {
        tmp += dataptr[i];          // add byte and save with carry
        res = tmp;                  // save into 8bit -> discard carry
        if (tmp > 0xFF)             // check if carry exitst
        {
            res++;                  // if carry, add one to lsb
        }
    }

    return ~res;                    // return bit inverted
}


void lin_SendPwm(uint16_t *data, uint8_t size, uint8_t id, char *lin_data)
{
	int i = 0;
	uint8_t x = 0;

	for(i=size-1; i>=0; i--)
	{
		lin_data[x] = *data >> i*8;
		x++;
	}
	lin_data[size] = lin_Checksum((uint8_t*) data, size, id);
	USART6 -> DR = lin_data[0];
}
	

void lin_SendFreq(uint32_t *data, uint8_t size, uint8_t id, char* lin_data)
{
	int i = 0;

	for(i=size-1; i>=0; i--)
	{
		lin_data[i] = *data >> i*8;
	}
	lin_data[size] = lin_Checksum((uint8_t*) data, size, id);
	USART6 -> DR = lin_data[0];
}
	

void lin_SendKeys(uint16_t *data, uint8_t size, uint8_t id, char* lin_data)
{
	int i = 0;
	uint8_t x = 0;

	for(i=size-1; i>=0; i--)
	{
		lin_data[x] = *data >> i*8;
		x++;
	}
	lin_data[size] = lin_Checksum((uint8_t*) data, size, id);
	USART6 -> DR = lin_data[0];
}
