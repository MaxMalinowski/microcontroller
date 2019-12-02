#include "lin.h"
#include "inttypes.h"


void lin_Init(void)
{
    // Configure ports (Clock, alt. function)
    // set clock for usart modul
    // configure usart as uart for lin
    // set baudrate
    // sending and receiving units
    // define interrupt happenings
    // set data format
    // turn on uart -> enable baudratetimer
    // uart in nvic
    // interrupthandler
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


}
