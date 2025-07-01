#include "gpio.h"
#include <stdint.h>

void gpio_init_pin(GPIO_TypeDef* port, uint8_t pin, uint8_t direction){
		if (port == GPIOA) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		}else if (port == GPIOB) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;	
		}else if (port == GPIOC) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
		}else if (port == GPIOD) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIODEN;
		}else if (port == GPIOE) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
		}else if (port == GPIOF) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
		}else if (port == GPIOG) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
		}else if (port == GPIOH) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
		}else if (port == GPIOI) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
		}else if (port == GPIOJ) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
		}else if (port == GPIOK) {
			RCC_AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
		}
		
		port->MODER &= ~(0x3 << (2*pin));
		if(direction == GPIO_OUTPUT){	
			port->MODER |= (0x1 << (2*pin));
		}
}

void gpio_write_pin(GPIO_TypeDef* port, uint8_t pin, uint8_t value){
	port->ODR = value ? (port->ODR | (1 << pin)) : (port->ODR & ~(1 << pin));
}

uint8_t gpio_read_pin(GPIO_TypeDef* port, uint8_t pin){
	return (port->IDR >> pin) & 1;
}