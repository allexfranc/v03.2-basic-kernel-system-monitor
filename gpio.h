#ifndef GPIO_H
#define GPIO_H

#include<stdint.h>

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

//Maybe move the RCC config to another part. 
#define RCC_AHB1ENR (*((volatile uint32_t *)0x40023830))
#define RCC_AHB1ENR_GPIOAEN (1 << 0)
#define RCC_AHB1ENR_GPIOBEN (1 << 1)
#define RCC_AHB1ENR_GPIOCEN (1 << 2)
#define RCC_AHB1ENR_GPIODEN (1 << 3)
#define RCC_AHB1ENR_GPIOEEN (1 << 4)
#define RCC_AHB1ENR_GPIOFEN (1 << 5)
#define RCC_AHB1ENR_GPIOGEN (1 << 6)
#define RCC_AHB1ENR_GPIOHEN (1 << 7)
#define RCC_AHB1ENR_GPIOIEN (1 << 8)
#define RCC_AHB1ENR_GPIOJEN (1 << 9)
#define RCC_AHB1ENR_GPIOKEN (1 << 10)

typedef struct {
    volatile uint32_t MODER;    // 0x00 - Configure pin direction
    volatile uint32_t OTYPER;   // 0x04 - Push-pull or open-drain
    volatile uint32_t OSPEEDR;  // 0x08 - Speed
    volatile uint32_t PUPDR;    // 0x0C - Pull-up/down
    volatile uint32_t IDR;      // 0x10 - Read input
    volatile uint32_t ODR;      // 0x14 - Write output
	volatile uint32_t BSRR;		// 0X18 - Reset register
	volatile uint32_t LCKR;		// 0X1C - Lock register
	volatile uint32_t AFRL;		// 0X20 - Alternate function low
	volatile uint32_t AFRH;		// 0X24 - Alternate function high
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *) 0x40020000)
#define GPIOB ((GPIO_TypeDef *) 0x40020400)
#define GPIOC ((GPIO_TypeDef *) 0x40020800)
#define GPIOD ((GPIO_TypeDef *) 0x40020C00)
#define GPIOE ((GPIO_TypeDef *) 0x40021000)
#define GPIOF ((GPIO_TypeDef *) 0x40021800)
#define GPIOG ((GPIO_TypeDef *) 0x40021C00)
#define GPIOH ((GPIO_TypeDef *) 0x40022000)
#define GPIOI ((GPIO_TypeDef *) 0x40022000)
#define GPIOJ ((GPIO_TypeDef *) 0x40022400)
#define GPIOK ((GPIO_TypeDef *) 0x40022800)

void gpio_init_pin(GPIO_TypeDef* port, uint8_t pin, uint8_t direction);
void gpio_write_pin(GPIO_TypeDef* port, uint8_t pin, uint8_t value);
uint8_t gpio_read_pin(GPIO_TypeDef* port, uint8_t pin);

#endif