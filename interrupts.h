#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define EXTI_PR		(*((volatile uint32_t *)0x40013C14))

void EXTI0_IRQHandler(void);

#endif