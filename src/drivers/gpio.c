#include "gpio.h"

/**
 * Configure LED pins
 */
void gpio_output_mode_init()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Pin = LD4_COLOR_BLUE | LD3_COLOR_RED
                  | LD5_COLOR_ORANGE | LD7_COLOR_GREEN;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &gpio);
}

/** 
 * Configure button pins
 */ 
void gpio_input_mode_init()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 
                  | GPIO_Pin_10 | GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOD, &gpio);
}

BitAction gpio_read_bit(GPIO_TypeDef* gpiox, uint16_t pin)
{
    if ((gpiox->IDR & pin) != (uint32_t)Bit_RESET) {
        return Bit_SET;
    } else {
        return Bit_RESET;
    }
}

void gpio_write_bit(GPIO_TypeDef* gpiox, uint16_t pin, BitAction value)
{
    if (value == Bit_RESET) {
        gpiox->ODR &= ~((uint16_t)pin);
    } else {
        gpiox->ODR |= ((uint16_t)pin);
    }
}
