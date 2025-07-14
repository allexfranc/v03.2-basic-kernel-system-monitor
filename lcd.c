#include "lcd.h"

static LCD_TypeDef lcd;

static void lcd_write_data_pins(uint8_t data){
	gpio_write_pin(lcd.port, lcd.d0, ((data >> 0) & 1));
	gpio_write_pin(lcd.port, lcd.d1, ((data >> 1) & 1));
	gpio_write_pin(lcd.port, lcd.d2, ((data >> 2) & 1));
	gpio_write_pin(lcd.port, lcd.d3, ((data >> 3) & 1));
	gpio_write_pin(lcd.port, lcd.d4, ((data >> 4) & 1));
	gpio_write_pin(lcd.port, lcd.d5, ((data >> 5) & 1));
	gpio_write_pin(lcd.port, lcd.d6, ((data >> 6) & 1));
	gpio_write_pin(lcd.port, lcd.d7, ((data >> 7) & 1));
}

void lcd_command(char i){
	lcd_write_data_pins(i);	
	gpio_write_pin(lcd.port, lcd.rs, 0);
	gpio_write_pin(lcd.port, lcd.en, 1);
	lcd_delay_ms(1);
	gpio_write_pin(lcd.port, lcd.en, 0);
	lcd_delay_ms(1);
}

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
				uint8_t en){
	lcd.port = port;
	lcd.d0 = d0;
	lcd.d1 = d1;
	lcd.d2 = d2;
	lcd.d3 = d3;
	lcd.d4 = d4;
	lcd.d5 = d5;
	lcd.d6 = d6;
	lcd.d7 = d7;
	lcd.rs = rs;
	lcd.en = en;
	
	gpio_write_pin(lcd.port, lcd.en, 0);
	lcd_delay_ms(100);
	lcd_command(LCD_CMD_WAKE_UP);
	lcd_delay_ms(30);
	lcd_command(LCD_CMD_WAKE_UP);
	lcd_delay_ms(10);
	lcd_command(LCD_CMD_WAKE_UP);
	lcd_delay_ms(10);
	lcd_command(LCD_CMD_FUNCTION_SET);
	lcd_command(LCD_CMD_SHIFT_CURSOR_LEFT);
	lcd_command(LCD_CMD_CURSOR_OFF);
	lcd_command(LCD_CMD_ENTRY_MODE);
	lcd_delay_ms(10);
	lcd_command(LCD_CMD_CLEAR);
	lcd_delay_ms(10);
}

void lcd_write_char(char i){
	lcd_write_data_pins(i);
	gpio_write_pin(lcd.port, lcd.rs, 1);
	gpio_write_pin(lcd.port, lcd.en, 1);
	lcd_delay_ms(1);
	gpio_write_pin(lcd.port, lcd.en, 0);
	lcd_delay_ms(1);
}

void lcd_clear_screen(void){
	lcd_command(LCD_CMD_CLEAR);
	lcd_delay_ms(10);
}

void lcd_write_string(const char* str){
	while(*str){
		lcd_write_char(*str++);
	}
}

void lcd_home(void){
	lcd_command(LCD_CMD_HOME);
	lcd_delay_ms(2);
}


void lcd_write_int(int value) {
    char buffer[12];
    int i = 11;
    int is_negative = 0;
    
    buffer[i--] = '\0';
    
    if (value == 0) {
        buffer[i] = '0';
        lcd_write_string(&buffer[i]);
        return;
    }
    
    if (value < 0) {
        is_negative = 1;
        value = -value;  
    }
    
    while (value > 0) {
        buffer[i--] = (value % 10) + '0';
        value /= 10;
    }
    
    if (is_negative) {
        buffer[i] = '-';
    } else {
        i++;  // Move back to first digit
    }
    
    // Display starting from first character
    lcd_write_string(&buffer[i]);
}

void lcd_write_uint(uint32_t value){
	char buffer[11];
    int i = 10;
    
    buffer[i--] = '\0';
    
    if (value == 0) {
        buffer[i] = '0';
        lcd_write_string(&buffer[i]);
        return;
    }
    
    while (value > 0) {
        buffer[i--] = (value % 10) + '0';
        value /= 10;
    }

	i++;  // Move back to first digit
    
    
    // Display starting from first character
    lcd_write_string(&buffer[i]);
}

void lcd_set_cursor(uint8_t row, uint8_t col){
	uint8_t address;
	switch(row){
		case 0: 
			address = 0x00 + col; 
			break;
		case 1: 
			address = 0x40 + col; 
			break;
		case 2: 
			address = 0x14 + col; 
			break;
		case 3: 	
			address = 0x54 + col; 
			break;
		default:
			return;
	}
	lcd_command(0x80 | address);
}

void lcd_delay_ms(uint32_t ms) {
    // Yes, crude counter loop
    // At 16MHz: ~16000 iterations per ms (tune this)
	// Was 16000 now 8000
    for(volatile uint32_t i = 0; i < ms * 1600; i++);
}
