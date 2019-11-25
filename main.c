#include "STM32F4xx.h"
#include "_mcpr_stm32f407.h"
#include "blinky.h"
#include "leds.h"
#include "display.h"
#include "keyboard.h"
#include "timer.h"
#include "frequency.h"


/*
 * Global variables
 */
volatile uint32_t delta = 0;
volatile uint32_t milliSec = 0;
volatile uint32_t mainTime = 0;
volatile uint32_t greenTime = 0;
volatile uint32_t backgroundTime = 0;
volatile uint8_t greenOn = 0;
volatile uint8_t userOn = 0;
volatile uint32_t capt_old = 0;
volatile uint32_t capt_new = 0;
volatile uint32_t frequency_Counted = 0;
volatile uint32_t frequency_Captured = 0;


/*
 * Interrupts
 */
void TIM7_IRQHandler() {
    TIM7->SR = 0;
    milliSec++;
}

void TIM8_BRK_TIM12_IRQHandler(void) {
    TIM12->SR = 0;
    capt_old = capt_new;
    capt_new = TIM12->CCR1;
}


/*
 * MAIN-Function
 */
int main(void) {
    /*
     * Initializations
     */
    mcpr_SetSystemCoreClock();
    led_Init();
    lcd_Init();
    lcd_ClearDisplay(0x00000);
    keyboard_Init();
    timer7_Init();
    timer12_Init();

    /*
     * Local variables
     */
    uint16_t old_keyboard = 0x0000;
    uint16_t new_keyboard = 0x0000;
    char keyboard[17];

    /*
     * loop forever
     */
    while (1) {
        mainTime = milliSec;

        /*
         * Frequency measurements
         */
        timer12_CounterInit();
        timer12_CheckCounter(milliSec, frequency_Counted);
        timer12_CaptureInit();
        timer12_CheckCapture(frequency_Captured, capt_old, capt_new);

        /*
         * Reading keyboard input
         * Dispaling input on lcd's and led's
         */
        old_keyboard = new_keyboard;
        new_keyboard = keyboard_Read();
        led_Write(new_keyboard);
        keyboard_Check(old_keyboard, new_keyboard, keyboard);

        /*
         * Checking timer7 conditions
         */
        timer7_CheckLed(&milliSec, &greenTime, &greenOn);
        timer7_CheckBackground(&milliSec, &backgroundTime, &userOn);

        /*
         * Waiting, if loop too fast
         */
        while (milliSec < (mainTime + 50)) {}
    }
}
