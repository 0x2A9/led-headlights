#include "gpio.h"

/**
 * Configure LED pins
 */
void gpio_output_mode_init()
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

BitAction gpio_read_bit(GPIO_TypeDef *gpiox, uint16_t pin)
{
    if ((gpiox->IDR & pin) != (uint32_t)Bit_RESET) {
        return Bit_SET;
    } else {
        return Bit_RESET;
    }
}

void gpio_write_bit(GPIO_TypeDef *gpiox, uint16_t pin, BitAction value)
{
    if (value == Bit_RESET) {
        gpiox->ODR &= ~((uint16_t)pin);
    } else {
        gpiox->ODR |= ((uint16_t)pin);
    }
}
