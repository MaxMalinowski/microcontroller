#include "inttypes.h"

void timer12_CounterInit(void);

void timer12_CaptureInit(void);

void timer12_CheckCounter(uint32_t* ms, uint32_t* freq_Count);

void timer12_CheckCapture(uint32_t *freq_Capt);