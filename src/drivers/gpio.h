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
    LD9_COLOR_BLUE   = GPIO_Pin_12,
    LD10_COLOR_RED   = GPIO_Pin_13,
    LD8_COLOR_ORANGE = GPIO_Pin_14,
    LD6_COLOR_GREEN  = GPIO_Pin_15,
    LD_COLOR_ALL     = (uint16_t)(LD3_COLOR_RED    | LD4_COLOR_BLUE 
                                | LD5_COLOR_ORANGE | LD6_COLOR_GREEN 
                                | LD7_COLOR_GREEN  | LD8_COLOR_ORANGE
                                | LD9_COLOR_BLUE   | LD10_COLOR_RED),
} ld_color_t;

void gpio_init_leds(void);

FlagStatus gpio_read_bit(const GPIO_TypeDef *gpiox, uint16_t pin);
void gpio_write_bit(GPIO_TypeDef *gpiox, uint16_t pin, FlagStatus value);

#endif // LED_HEADLIGHTS_GPIO_H
