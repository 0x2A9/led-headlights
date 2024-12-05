#include "stm32f30x_it.h"

CanRxMsg rx_msg;

void USB_HP_CAN1_TX_IRQHandler()
{
}

void CAN1_RX1_IRQHandler()
{
    gpio_write_bit(GPIOE, GPIO_Pin_8, Bit_SET);

    can_read(CAN1, CAN_FIFO1, &rx_msg);

    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);
}
