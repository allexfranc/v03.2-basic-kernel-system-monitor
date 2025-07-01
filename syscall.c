#include "syscall.h"
#include "scheduler.h"
#include "led.h"

static uint8_t get_svs_number(uint32_t* stack){
	  // 1. Get the saved PC from the stack frame. 
    //    It holds the address of the instruction AFTER the SVC call.
    uint8_t* pc = (uint8_t*)stack[6];

    // 2. The SVC instruction is 2 bytes long. To find the start of the
    //    instruction, we must look 2 bytes before the saved PC. The first
    //    byte at that location is the immediate value.
	
	return pc[-2];
}

static void trigger_PendSV(void){
	SCB_ICSR |= (1 << 28);  // Trigger PendSV
}

void SVC_Handler(void){	
	// Get current task's stack (was in PSP)
	uint32_t* psp_stack;
	__asm volatile ("MRS %0, psp" : "=r" (psp_stack));
	
	uint8_t svc_num = get_svs_number(psp_stack);
	
	switch(svc_num) {
		case SYS_YIELD:
			// Just trigger a context switch
            trigger_PendSV();  // Trigger PendSV
			break;
		
		case SYS_SLEEP:
			task_sleep_ms(get_current_task(), psp_stack[0]);
			trigger_PendSV();
			break;
		
		case SYS_GET_TASK_ID:
			//This is how we pass the return value without calling return, 
			//ie, we are returning from an exception
			psp_stack[0] = get_current_task(); 
            break;
            
        case SYS_LED_CONTROL:
            // param1 = LED number, param2 = state
            uint32_t led_id = psp_stack[0];
            uint32_t state = psp_stack[1];  // R1 = second parameter
			
			led_set(led_id, state);
            
            break;
	}
}

void sys_yield(void){
	__asm volatile ("SVC #0");
}

void sys_sleep(uint32_t ms){
	 __asm volatile (
		"MOV r0, %0    \n"  // Put ms parameter in R0
		"SVC #1        \n"  // Trigger system call #1
		:                   // No outputs
		: "r"(ms)          // Input: ms
		: "r0"             // We're modifying R0
	);
}

uint8_t sys_get_task_id(void) {
    uint32_t result;
    
    __asm volatile (
        "SVC #2        \n"
        "MOV %0, r0    \n"  // Explicitly copy R0 to result
        : "=r"(result)
        :
        : "r0"
    );
    
    return (uint8_t)result;
}

void sys_led_control(uint8_t led, uint8_t state){
	 __asm volatile (
		"MOV r0, %0    \n"  // LED number in R0
		"MOV r1, %1    \n"  // State in R1
		"SVC #3        \n"  // Trigger system call #3
		:                   // No outputs
		: "r"(led), "r"(state)  // Inputs
		: "r0", "r1"           // We're modifying R0, R1
	);
}