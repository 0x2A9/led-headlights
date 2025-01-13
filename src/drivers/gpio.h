#ifndef LED_HEADLIGHTS_GPIO_H
#define LED_HEADLIGHTS_GPIO_H

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"

typedef enum {
    LD4_COLOR_BLUE   = GPIO_Pin_8,
    LD3_COLOR_RED    = GPIO_Pin_9,
    LD5_COLOR_ORANGE = GPIO_Pin_10,
    LD7_COLOR_GREEN  = GPIO_Pin_11,
} ld_color_t;

void gpio_output_mode_init();
void gpio_input_mode_init();
void gpio_i2c_mode_init();
BitAction gpio_read_bit(GPIO_TypeDef* gpiox, uint16_t pin);
void gpio_write_bit(GPIO_TypeDef* gpiox, uint16_t pin, BitAction value);

#endif // LED_HEADLIGHTS_GPIO_H
