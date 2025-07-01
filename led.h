#include <stdint.h>

#define LED_GREEN 	0
#define LED_BLUE 	1
#define LED_ORANGE 	2
#define LED_RED 	3

#define LED_ON		1
#define LED_OFF		0

void led_init_all(void);
void led_set(uint8_t led_id, uint8_t state);