#ifndef LED_HEADLIGHTS_STM32F30X_IT_H
#define LED_HEADLIGHTS_STM32F30X_IT_H

#include "stm32f30x_can.h"
#include "drivers/gpio.h"
#include "drivers/can.h"

extern CanRxMsg rx_msg;

void USB_HP_CAN1_TX_IRQHandler();
void CAN1_RX1_IRQHandler();

#endif // LED_HEADLIGHTS_STM32F30X_IT_H
