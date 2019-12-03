#include "lin.h"
#include "inttypes.h"


void lin_Init(void)
{
    RCC -> AHB1ENR |= 0x00000004;               // enable clock for GPIOC
    GPIOC -> MODER |= 0x000000A0;               // set Pin 6, 7 to Alternate Mode
    GPIOC -> AFR[0] |= 0x76000000;              // select alternate functions
    RCC -> APB1ENR |= 0x00000020;               // enable clock for usart6
    USART6 -> BRR = 0x00001117;                 // set baudrate 273,4375
    USART6 -> CR1 |= 0x0000206A;                // enable RE, TE, RXNEIE, TCIE, UE
    USART6 -> CR2 |= 0x00004040;                // enable LINEN, LBDIE
    NVIC_SetPriority(USART6_IRQn, 0);           // set interrupt priority
    NVIC_Enable_IRQ(USART_IRQn);                // enable interrupt
}


uint8_t lin_Checksum(uint8_t* dataptr, uint8_t size, uint8_t id)
{
    uint8_t i = 0;
    uint8_t res = 0;
    uint16_t tmp = id;

    for(i = 0; i < size; i++)
    {
        tmp =+ dataptr[i];          // add byte and save with carry
        res = tmp;                  // save into 8bit -> discard carry
        if (tmp > 0xFF)             // check if carry exitst
        {
            res++;                  // if carry, add one to lsb
        }
    }

    return ~res;                    // return bit inverted
}



void lin_PackData(uint8_t id, uint8_t* dataptr, uint8_t size, char* lin_result)
{
    uint8_t i = 0;
    for (i = 0; i < size; i++)
    {
        lin_result[i] = dataptr[i];
    }
    lin_result[size] = lin_Checksum(dataptr, size, id);
}
