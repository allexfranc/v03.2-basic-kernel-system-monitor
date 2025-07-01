#include "led.h"
#include "gpio.h"

static const struct {
    GPIO_TypeDef* port;
    uint8_t pin;
} led_hardware[] = {
    [LED_GREEN]  = {GPIOD, 12},
    [LED_ORANGE] = {GPIOD, 13},
    [LED_RED]    = {GPIOD, 14},
    [LED_BLUE]   = {GPIOD, 15}
};

void led_init_all(void){
	for (int i = 0; i < 4; i++) {
        gpio_init_pin(led_hardware[i].port, led_hardware[i].pin, GPIO_OUTPUT);
    }
}

void led_set(uint8_t led_id, uint8_t state){
	if (led_id >= 4) return;
    if (state != LED_ON && state != LED_OFF) return;
	gpio_write_pin(led_hardware[led_id].port, led_hardware[led_id].pin, state);
}