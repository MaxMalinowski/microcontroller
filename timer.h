#include "inttypes.h"

void timer7_Init(void);

void timer7_CheckLed(volatile uint32_t* ms, volatile uint32_t* gt, volatile uint8_t* go);

void timer7_CheckBackground(volatile uint32_t* ms, volatile uint32_t* bt, volatile uint8_t* uo);



