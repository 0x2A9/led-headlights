#ifndef LED_HEADLIGHTS_GPIO_H
#define LED_HEADLIGHTS_GPIO_H

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"

void gpio_output_mode_init();
void gpio_input_mode_init();
void gpio_i2c_mode_init();
void gpio_can_mode_init();
BitAction gpio_read_bit(GPIO_TypeDef* GPIOx, uint16_t pin);
void gpio_write_bit(GPIO_TypeDef* GPIOx, uint16_t pin, BitAction value);

#endif // LED_HEADLIGHTS_GPIO_H
