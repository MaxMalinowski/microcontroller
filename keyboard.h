#include "inttypes.h"

uint16_t keyboard_Read(void);

void Keyboard_Check(uint16_t old_key, uint16_t new_key, char* keyboard);

void keyboard_Init(void);

void Short2Bitstring(uint16_t keys, char* array);

void Keyboard_Main(void);

