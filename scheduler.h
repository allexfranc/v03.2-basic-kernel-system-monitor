#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>


#define STACK_SIZE 128
#define NUM_TASKS 3

typedef enum {
	TASK_READY,
	TASK_RUNNING,
	TASK_SLEEPING
}task_state_t;

typedef struct{
	uint32_t* stack_pointer;
	task_state_t state;
	uint32_t wake_tick;
} task_t;

void scheduler_init(void);
void scheduler_start(void);

// Our tasks
void idle_task(void);
void task_green(void);
void task_blue(void);

// Handlers for interrupts
void PendSV_Handler(void);
void SysTick_Handler(void);

// Set Task Sleep
void task_sleep_ms(uint8_t task_num, uint32_t sleep_ms);

//getter and setter
void set_task_state(uint8_t task_num, task_state_t state);
task_state_t get_task_state(uint8_t task_num);
uint8_t get_current_task(void);

#endif