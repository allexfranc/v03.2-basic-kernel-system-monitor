// startup.c - The not so minimum to boot
#include <stdint.h>
#include "scheduler.h"

// Cortex-M Processor interrupts (0-15)
void Reset_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
// STM32 interrupts (16-)
void EXTI0_IRQHandler(void);

// The vector table - MUST be at exactly 0x08000000
__attribute__((section(".vectors")))
const unsigned int vector_table[] = {
	0x20020000,                      // 0: Stack pointer
    (unsigned int)Reset_Handler,     // 1: Reset
    0,                               // 2: NMI
    (unsigned int)HardFault_Handler, // 3: HardFault
    0,                               // 4: MemManage
    0,                               // 5: BusFault
    0,                               // 6: UsageFault
    0, 0, 0, 0,                      // 7-10: Reserved
    (unsigned int)SVC_Handler,       // 11: SVCall
    0,                               // 12: Debug Monitor
    0,                               // 13: Reserved
    (unsigned int)PendSV_Handler,    // 14: PendSV
    (unsigned int)SysTick_Handler,   // 15: SysTick
	
	 // External Interrupts start here (positions 16+)
    0,                               // 16: WWDG (IRQ0)
    0,                               // 17: PVD (IRQ1)
    0,                               // 18: TAMP_STAMP (IRQ2)
    0,                               // 19: RTC_WKUP (IRQ3)
    0,                               // 20: FLASH (IRQ4)
    0,                               // 21: RCC (IRQ5)
    (unsigned int)EXTI0_IRQHandler,  // 22: EXTI0 (IRQ6) <-- Here!
};

//TODO
static void system_clock_init(void) {
    // 1. Enable HSE (external crystal)
    // 2. Wait for HSE ready
    // 3. Configure PLL (HSE * multipliers = 168MHz)
    // 4. Enable PLL and wait
    // 5. Switch system clock to PLL
    // 6. Update SystemCoreClock variable
}

// This is where we start!
void Reset_Handler(void) {
	// 1. Set up clocks FIRST (before anything uses them!)
    system_clock_init(); 
	
	
	
	// Copy .data from flash to RAM
    extern uint32_t _sdata, _edata, _sidata;
    uint32_t *src = &_sidata;  // Source in flash
    uint32_t *dst = &_sdata;   // Destination in RAM
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    
    // Zero .bss
    extern uint32_t _sbss, _ebss;
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
	
    // Jump to main
    extern int main(void);
    main();
    
    // If main returns, hang here
    while(1);
}

void HardFault_Handler(void) {
    while(1) {
       // handle faults here
    }
}
