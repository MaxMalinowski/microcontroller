#include "inttypes.h"

void timer12_Init(void);

void timer12_CounterInit(void);

void timer12_CaptureInit(void);

void timer12_CheckCounter(volatile uint32_t* ms, volatile uint32_t* freq_Count);

void timer12_CheckCapture(volatile uint32_t* freq_Capt, volatile uint32_t* capt_old, volatile uint32_t* capt_new);

