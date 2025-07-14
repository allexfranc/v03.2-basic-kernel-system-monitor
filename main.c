#include <stdint.h>
#include "scheduler.h"
#include "gpio.h"
#include "syscall.h"
#include "led.h"
#include "lcd.h"

// Simple tasks - just toggle LEDs
void task_green(void) {	
	while(1){
		sys_led_control(LED_GREEN, LED_ON);
        for(volatile int i = 0; i < 100000; i++); // Burn some CPU to Show on LCD
		sys_led_control(LED_GREEN, LED_OFF);
		sys_sleep(500);
	}
}

void task_blue(void) {
	while(1){
		sys_led_control(LED_BLUE, LED_ON);
        for(volatile int i = 0; i < 100000; i++); // Burn some CPU
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

// Add this helper function before task_system_monitor
void lcd_write_2digit(uint32_t num, uint8_t space) {
    if (num < 10) {
		if (space){
			lcd_write_char(' ');
		}
		else{
			lcd_write_char('0');
		}
    }
    lcd_write_uint(num);
}

void task_system_monitor(void){
    // Static variables to track deltas
    static uint32_t last_check_ticks = 0;
    static uint32_t task_last_ticks[4] = {0};
    static uint8_t first_run = 1;
    
    while(1){
        uint32_t system_ticks = sys_ticks();
        
        lcd_set_cursor(0, 0);
        lcd_write_string("AllexOS   U:");
        
        uint32_t total_sec = system_ticks / 1000;
        uint32_t hours = total_sec / 3600;
        uint32_t min = (total_sec % 3600) / 60;
        uint32_t sec = total_sec % 60;
        
        lcd_write_2digit(hours, 0);
        lcd_write_char(':');
        lcd_write_2digit(min, 0);
        lcd_write_char(':');
        lcd_write_2digit(sec, 0);
        
        // Calculate CPU percentages over window
        uint32_t percentages[4] = {0};
        
        if (!first_run) {
            uint32_t delta_time = system_ticks - last_check_ticks;
            if (delta_time > 0) {
                for (int i = 0; i < 4; i++) {
                    uint32_t task_current_ticks = sys_task_ticks(i);
                    uint32_t task_delta_ticks = task_current_ticks - task_last_ticks[i];
                    percentages[i] = (task_delta_ticks * 100 + delta_time/2) / delta_time; // Round up
                    task_last_ticks[i] = task_current_ticks;
                }
            }
        } else {
            // First run - use total time
            for (int i = 0; i < 4; i++) {
                task_last_ticks[i] = sys_task_ticks(i);
                percentages[i] = (task_last_ticks[i] * 100 + system_ticks/2) / system_ticks;
            }
            first_run = 0;
        }
		
        last_check_ticks = system_ticks;
        
        lcd_set_cursor(1, 0);
		
		lcd_write_string("I:");
        lcd_write_2digit(percentages[IDLE_TASK_NUM], 1);
        lcd_write_string("%G:");
        lcd_write_2digit(percentages[GREEN_TASK_NUM], 1);
        lcd_write_string("%B:");
        lcd_write_2digit(percentages[BLUE_TASK_NUM], 1);
        lcd_write_string("%M:");
        lcd_write_2digit(percentages[MONITOR_TASK_NUM], 1);
        lcd_write_char('%');
        
        // Third line: System ticks
        lcd_set_cursor(2, 0);
        lcd_write_string("Ticks: ");
        lcd_write_uint(system_ticks);
        
        // Fourth line: Context switches
        lcd_set_cursor(3, 0);
        lcd_write_string("Switches: ");
        lcd_write_uint(sys_context_switches());

        
        sys_sleep(150);  // Update 20 times per second
    }
}

void init_lcd(){
	gpio_init_pin(GPIOA, 0, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 1, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 2, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 3, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 4, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 5, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 6, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 7, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 8, GPIO_OUTPUT);
	gpio_init_pin(GPIOA, 9, GPIO_OUTPUT);
	
	lcd_init(
		GPIOA,
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8,
		9
	);
}

int main(void) {
	//Initialize LEDs
	led_init_all();
	
	// Initialize LCD
	init_lcd();
	
	gpio_init_pin(GPIOD, 5, GPIO_INPUT);
	GPIOD->PUPDR |= (1 << (5 * 2));  // Pull-up on PD5
	
    // Initialize scheduler
    scheduler_init();
    
    // Start scheduler
    scheduler_start();
    
    // Never reached
    while(1);
}