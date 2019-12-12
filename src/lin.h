#include "inttypes.h"

void lin_Init(void);

uint8_t lin_Checksum(uint8_t* dataptr, uint8_t size, uint8_t id);

void lin_Send(uint32_t *data, uint8_t size, uint8_t id, char *lin_data);
