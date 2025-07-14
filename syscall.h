#ifndef SYSCALL_H
#define SYSCALL_H
#include <stdint.h>

#define SCB_ICSR 				(*((volatile uint32_t *)0xE000ED04))
#define SYS_YIELD				0
#define SYS_SLEEP				1
#define SYS_GET_TASK_ID			2
#define SYS_LED_CONTROL			3
#define SYS_TICKS				4
#define SYS_CONTEXT_SWITCHES	5
#define SYS_TASK_TICKS			6


// System call functions (what tasks will call)
void sys_yield(void);
void sys_sleep(uint32_t ms);
uint8_t sys_get_task_id(void);
void sys_led_control(uint8_t led, uint8_t state);
uint32_t sys_ticks(void);
uint32_t sys_context_switches(void);
uint32_t sys_task_ticks(uint8_t task_num);

// SVC Handler
void SVC_Handler(void);

#endif