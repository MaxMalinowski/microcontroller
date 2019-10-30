#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "Blinky.h"


//-----------------------------------------------------
//MAIN
//-----------------------------------------------------
int main(void) {
	mcpr_SetSystemCoreClock();
	
	blinky_main();
}
