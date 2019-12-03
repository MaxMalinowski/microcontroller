#include "inttypes.h"

void timer12_Init(void);

void timer12_CounterInit(void);

void timer12_CaptureInit(void);

void timer12_CheckCounter(volatile uint32_t* ms, uint32_t* freq_Count);

void timer12_CheckCapture(uint32_t* freq_Capt, volatile uint32_t* capt_old, volatile uint32_t* capt_new, volatile uint8_t* tim12_count);

void int2Bitstring(uint32_t keys, char* array);
