#include "inttypes.h"

void lin_Init(void);

uint8_t lin_Checksum(uint8_t* dataptr, uint8_t size, uint8_t id);

void lin_SendPwm(uint8_t *data, uint8_t size, uint8_t id, char* lin);
	
void lin_SendFreq(uint32_t *data, uint8_t size, uint8_t id, char* lin);
	
void lin_SendKeys(uint16_t *data, uint8_t size, uint8_t id, char* lin);
