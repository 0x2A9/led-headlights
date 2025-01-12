#ifndef LED_HEADLIGHTS_STM32F30X_IT_H
#define LED_HEADLIGHTS_STM32F30X_IT_H

#include "drivers/gpio.h"
#include "drivers/can.h"
#include "drivers/i2c.h"
#include "stm32f30x_can.h"

void USB_HP_CAN1_TX_IRQHandler();
void CAN1_RX1_IRQHandler();
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);

#endif // LED_HEADLIGHTS_STM32F30X_IT_H
