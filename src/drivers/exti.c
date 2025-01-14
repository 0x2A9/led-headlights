#include "exti.h"

void exti_init() 
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
    gpio.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 
                   | GPIO_Pin_10 | GPIO_Pin_11;

    GPIO_Init(GPIOD, &gpio);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
      
    /* Connect EXTI Lines to PD8, PD9, PD10, PD11 pins */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource8);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource9);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource10);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource11);

    EXTI_InitTypeDef exti;

    exti.EXTI_Line    = EXTI_Line8;
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);

    exti.EXTI_Line    = EXTI_Line9;
    EXTI_Init(&exti);
  
    exti.EXTI_Line    = EXTI_Line10;
    EXTI_Init(&exti);

    exti.EXTI_Line    = EXTI_Line11;
    EXTI_Init(&exti);

    NVIC_InitTypeDef nvic;
    
    /* Enable and set EXTI Interrupt to the lowest priority */
    nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
    nvic.NVIC_IRQChannelSubPriority        = 0x0F;
    nvic.NVIC_IRQChannelCmd                = ENABLE;
    nvic.NVIC_IRQChannel                   = EXTI9_5_IRQn; /* Line 5-9 */
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel                   = EXTI15_10_IRQn; /* Line 10-15 */
    NVIC_Init(&nvic);
}
