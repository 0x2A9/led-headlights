#include "gpio.h"

void gpio_init_leds(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    gpio.GPIO_Pin   = LD3_COLOR_RED    | LD4_COLOR_BLUE 
                    | LD5_COLOR_ORANGE | LD6_COLOR_GREEN 
                    | LD7_COLOR_GREEN  | LD8_COLOR_ORANGE
                    | LD9_COLOR_BLUE   | LD10_COLOR_RED;
    GPIO_Init(GPIOE, &gpio);
}

FlagStatus gpio_read_bit(GPIO_TypeDef *gpiox, uint16_t pin)
{
    if ((gpiox->IDR & pin) != (uint32_t)RESET) {
        return SET;
    } else {
        return RESET;
    }
}

void gpio_write_bit(GPIO_TypeDef *gpiox, uint16_t pin, FlagStatus value)
{
    if (value == RESET) {
        gpiox->ODR &= ~((uint16_t)pin);
    } else {
        gpiox->ODR |= ((uint16_t)pin);
    }
}
