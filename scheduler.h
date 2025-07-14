#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>


#define STACK_SIZE 			128
#define NUM_TASKS 			4	
#define IDLE_TASK_NUM		0
#define GREEN_TASK_NUM		1
#define BLUE_TASK_NUM		2
#define MONITOR_TASK_NUM	3

typedef enum {
	TASK_READY,
	TASK_RUNNING,
	TASK_SLEEPING
}task_state_t;

typedef struct{
	uint32_t* stack_pointer;
	task_state_t state;
	uint32_t wake_tick;
	uint32_t run_ticks;
} task_t;

void scheduler_init(void);
void scheduler_start(void);

// Our tasks
void idle_task(void);
void task_green(void);
void task_blue(void);
void task_system_monitor(void);

// Handlers for interrupts
void PendSV_Handler(void);
void SysTick_Handler(void);

// Set Task Sleep

//getter and setter
// Task services
void task_sleep_ms(uint8_t task_num, uint32_t sleep_ms);
void set_task_state(uint8_t task_num, task_state_t state);
task_state_t get_task_state(uint8_t task_num);
uint8_t get_current_task(void);
uint32_t get_task_ticks(uint8_t task_num);

// Kernel services
uint32_t get_system_ticks(void);
uint32_t get_context_switches(void);

#endif