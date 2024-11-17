#ifndef LED_HEADLIGHTS_CAN_H
#define LED_HEADLIGHTS_CAN_H

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_can.h"

uint8_t can_init();
uint8_t can_write(CanTxMsg* msg);

#endif // LED_HEADLIGHTS_CAN_H
