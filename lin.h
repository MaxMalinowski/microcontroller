#include "inttypes.h"

void lin_Init(void);

uint8_t lin_Checksum(uint8_t* dataptr, uint8_t size, uint8_t id);

void lin_PackData(uint8_t id, uint8_t* dataptr, uint8_t size, char* lin_result);

