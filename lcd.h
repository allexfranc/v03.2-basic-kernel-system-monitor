#ifndef LCD_H
#define LCD_H

#include<stdint.h>
#include "gpio.h"

#define LCD_CMD_CLEAR           	0x01
#define LCD_CMD_HOME            	0x02  
#define LCD_CMD_DISPLAY_ON      	0x0C
#define LCD_CMD_DISPLAY_OFF     	0x08
#define LCD_CMD_WAKE_UP         	0x30
#define LCD_CMD_FUNCTION_SET    	0x38  // 8-bit, 2-line, 5x8 font
#define LCD_CMD_ENTRY_MODE      	0x06  // Increment, no shift
#define LCD_CMD_CURSOR_OFF      	0x0C  // Display on, cursor off
#define LCD_CMD_CURSOR_ON       	0x0E  // Display on, cursor on, no blink
#define LCD_CMD_CURSOR_BLINK    	0x0F  // Display on, cursor on, blink
#define LCD_CMD_SHIFT_CURSOR_LEFT  	0x10  // Move cursor left


typedef struct {
    GPIO_TypeDef *port;
    uint8_t d0;    
    uint8_t d1;    
    uint8_t d2;
    uint8_t d3;
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
    uint8_t d7;
    uint8_t rs;    
    uint8_t en;
} LCD_TypeDef;

void lcd_write_int(int value);
void lcd_write_uint(uint32_t value);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_home(void);
void lcd_write_string(const char* str);
void lcd_write_char(char i);
void lcd_command(char i);
void lcd_clear_screen(void);
void lcd_init(GPIO_TypeDef *port, 
				uint8_t d0, 
				uint8_t d1, 
				uint8_t d2, 
				uint8_t d3, 
				uint8_t d4, 
				uint8_t d5, 
				uint8_t d6, 
				uint8_t d7,
				uint8_t rs,
				uint8_t en);
				
void lcd_delay_ms(uint32_t ms);

#endif