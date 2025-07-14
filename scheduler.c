#include "scheduler.h"

// Separate stack for each task
static uint32_t idle_stack[STACK_SIZE];
static uint32_t green_stack[STACK_SIZE];
static uint32_t blue_stack[STACK_SIZE];
static uint32_t system_monitor_stack[STACK_SIZE];

// Task control blocks
static task_t tasks[NUM_TASKS];
static uint8_t current_task = 0;

// SysTick registers
#define SYST_CSR   (*((volatile uint32_t *)0xE000E010))
#define SYST_RVR   (*((volatile uint32_t *)0xE000E014))
#define SYST_CVR   (*((volatile uint32_t *)0xE000E018))

// SCB register for PendSV
#define SCB_ICSR   (*((volatile uint32_t *)0xE000ED04))

static uint32_t system_ticks = 0;
static uint8_t scheduler_countdown = 10;
static uint32_t task_size = sizeof(task_t);
static uint32_t context_switches = 0;

// SysTick triggers context switch
void SysTick_Handler(void) {
	system_ticks++;
	
	// Count ticks for current running task
	if(tasks[current_task].state == TASK_RUNNING){
			tasks[current_task].run_ticks++;
	}
	
   // Search for sleeping tasks and wake them up
    for(int i = 0; i < NUM_TASKS; i++) {
        if (tasks[i].state == TASK_SLEEPING && 
            system_ticks >= tasks[i].wake_tick) {
            tasks[i].state = TASK_READY;
        }
    }
	
	//Triggers context switch every 10 ticks (10ms)
	if(--scheduler_countdown == 0){ //10 to 30 times more efficient than using sytem_ticks%10 == 0
		scheduler_countdown = 10;
		SCB_ICSR = (1 << 28);  // Set PendSV
	}
}

// Initialize a task stack
static uint32_t* init_stack(uint32_t* stack_top, void (*handler)(void)) {
	
    uint32_t* sp = &stack_top[STACK_SIZE - 1];
   
    // ARM Cortex-M stack frame (hardware pushed)
    *(sp) = 0x01000000;             // xPSR (thumb bit set)
    *(--sp) = (uint32_t)handler;    // PC
    *(--sp) = 0xFFFFFFFD;           // LR (return to thread mode, use PSP)
    *(--sp) = 0;                    // R12
    *(--sp) = 0;                    // R3
    *(--sp) = 0;                    // R2
    *(--sp) = 0;                    // R1
    *(--sp) = 0;                    // R0
    
    // Software saved registers
    *(--sp) = 0;                    // R11
    *(--sp) = 0;                    // R10
    *(--sp) = 0;                    // R9
    *(--sp) = 0;                    // R8
    *(--sp) = 0;                    // R7
    *(--sp) = 0;                    // R6
    *(--sp) = 0;                    // R5
    *(--sp) = 0;                    // R4
    
    return sp;
}

void scheduler_init(void) {
    // Initialize each task
    tasks[0].stack_pointer = init_stack(idle_stack, idle_task);
	tasks[0].state = TASK_READY;
	tasks[0].wake_tick = 0;
    tasks[1].stack_pointer = init_stack(green_stack, task_green);
	tasks[1].state = TASK_READY;
	tasks[1].wake_tick = 0;
    tasks[2].stack_pointer = init_stack(blue_stack, task_blue);
	tasks[2].state = TASK_READY;
    tasks[2].wake_tick = 0; 
	tasks[3].stack_pointer = init_stack(system_monitor_stack, task_system_monitor);
	tasks[3].state = TASK_READY;
    tasks[3].wake_tick = 0;
    current_task = 0;
}

void scheduler_start(void) {
    // Set initial PSP
    __asm volatile ("MSR psp, %0" : : "r" (tasks[0].stack_pointer));
    
    // Use PSP for thread mode - FIXED!
    uint32_t control = 2;
    __asm volatile ("MSR control, %0" : : "r" (control));
    __asm volatile ("ISB");
    
    // Configure SysTick for 1ms
    SYST_RVR = 16000 - 1;  // 1ms at 16MHz
    SYST_CVR = 0;
    SYST_CSR = 7;  // Enable, interrupt, processor clock
    
    // Enable interrupts
    __asm volatile ("cpsie i");
    
    // Should never return
    while(1);
}

uint8_t switch_task(void){
	uint8_t next_task = current_task;
	uint8_t checked = 0;
	context_switches++;

	
	do{
		next_task = ((next_task + 1)%NUM_TASKS);
		checked++;
		
		if (checked >= NUM_TASKS){
			next_task = 0;
			break;
		}
		
	} while (tasks[next_task].state != TASK_READY);
	
	tasks[next_task].state = TASK_RUNNING;

	if (tasks[current_task].state == TASK_RUNNING) {
        tasks[current_task].state = TASK_READY;
    }
	
	return next_task;
}

// PendSV does the actual switch
__attribute__((naked))
void PendSV_Handler(void) {
    __asm volatile(
        // Save current context
        "mrs r0, psp              \n"
        "isb                      \n"
        
        // Save r4-r11
        "stmdb r0!, {r4-r11}      \n"
        
        // Save current task's SP
        "ldr r1, =current_task    \n"
        "ldrb r2, [r1]            \n"  // Get current task number
        "ldr r3, =tasks           \n"  // Get tasks array base
		
		"ldr r1, =task_size       \n"  // Load task_size
		"ldr r1, [r1]			  \n"
		"mul r2, r2, r1           \n"  // r2 = r2 * task_size
        "str r0, [r3, r2]         \n"  // Save SP - offsetted tasks base array by task_number
        
		// Call C function to get next task  
		"push {lr}                \n"  
		"bl switch_task           \n"  
		"pop {lr}                 \n"
		"ldr r1, =current_task    \n"  
		"strb r0, [r1]            \n"  // Save returned task number
		"mov r2, r0               \n"  // Task number in r2
		"ldr r3, =tasks           \n"  // Reload tasks array base
        
        // Get next task's SP
        "ldr r1, =task_size   	  \n"  // Load task_size
		"ldr r1, [r1]			  \n"
		"mul r2, r2, r1           \n"  // r2 = r2 * task_size
        "ldr r0, [r3, r2]         \n"  // Load new SP - - offsetted tasks base array by task_number
        
        // Restore r4-r11
        "ldmia r0!, {r4-r11}      \n"
        
        // Set PSP to new task
        "msr psp, r0              \n"
        "isb                      \n"
        
        // Return
        "orr lr, lr, #0x04        \n"  // Ensure return to thread mode
        "bx lr                    \n"
    );
}

void task_sleep_ms(uint8_t task_num, uint32_t sleep_ms){
	   // Disable interrupts to avoid race conditions!
    __asm volatile ("cpsid i");
	 if (task_num < NUM_TASKS) {
		tasks[task_num].wake_tick = system_ticks + sleep_ms;
        tasks[task_num].state = TASK_SLEEPING;
    }
	   // Enable interrupts
    __asm volatile ("cpsie i");
}

// Task implementations
void idle_task(void) {
    while(1) {
        __asm volatile ("WFI");
    }
}

// Getters and Setters
void set_task_state(uint8_t task_num, task_state_t state) {
    if (task_num < NUM_TASKS) {
        tasks[task_num].state = state;
    }
}

task_state_t get_task_state(uint8_t task_num) {
    return tasks[task_num].state;
}

uint8_t get_current_task(void){
	return current_task;
}

uint32_t get_system_ticks(void){
	return system_ticks;
}

uint32_t get_context_switches(void) {
    return context_switches;
}

uint32_t get_task_ticks(uint8_t task_num){
	if(task_num >= NUM_TASKS){
		return 0;
	}
	return tasks[task_num].run_ticks;
}
