#include "inttypes.h"

void timer7_Init(void);

void timer7_CheckLed(volatile uint32_t* ms, uint32_t* gt, uint8_t* go);

void timer7_CheckBackground(volatile uint32_t* ms, uint32_t* bt, uint8_t* uo);



