#include <stdint.h>
#include "scheduler.h"
#include "gpio.h"
#include "syscall.h"
#include "led.h"

// Simple tasks - just toggle LEDs
void task_green(void) {	
	while(1){
		sys_led_control(LED_GREEN, LED_ON);
		sys_sleep(500);
        sys_led_control(LED_GREEN, LED_OFF);
		sys_sleep(500);
	}
}

void task_blue(void) {
	while(1){
		sys_led_control(LED_BLUE, LED_ON);   // Use syscalls
		sys_sleep(500);
        sys_led_control(LED_BLUE, LED_OFF);
		sys_sleep(500);
	} 
}

void task_orange(void){
	while(1){
		sys_led_control(LED_ORANGE, LED_ON);
		sys_sleep(500);
		sys_led_control(LED_ORANGE, LED_OFF);
		sys_sleep(500);
	} 
}

void task_red(void){
	while(1){
		sys_led_control(LED_RED, LED_ON);
		sys_sleep(500);
		sys_led_control(LED_RED, LED_OFF);
		sys_sleep(500);
	} 
}

int main(void) {
	//Initialize LEDs
	led_init_all();
	
    // Initialize scheduler
    scheduler_init();
    
    // Start scheduler
    scheduler_start();
    
    // Never reached
    while(1);
}