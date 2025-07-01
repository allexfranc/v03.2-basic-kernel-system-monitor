#include "interrupts.h"
#include "gpio.h"
#include "scheduler.h"

// Global flag for button press
volatile uint8_t button_pressed = 0;

void EXTI0_IRQHandler(void) {
    // Clear interrupt flag
    EXTI_PR = (1 << 0);
}