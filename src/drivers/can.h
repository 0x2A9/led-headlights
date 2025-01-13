#ifndef LED_HEADLIGHTS_CAN_H
#define LED_HEADLIGHTS_CAN_H

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_can.h"
#include "stm32f30x_misc.h"

extern CanTxMsg can_tx_msg;
extern CanRxMsg can_rx_msg;

void can_init();
void can_filter_init();
uint8_t can_read(CAN_TypeDef* canx, uint8_t fifo_number, CanRxMsg* msg);
uint8_t can_write(CAN_TypeDef* canx, CanTxMsg* msg);

#endif // LED_HEADLIGHTS_CAN_H
