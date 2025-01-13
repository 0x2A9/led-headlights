#include "stm32f30x_it.h"

void USB_HP_CAN1_TX_IRQHandler()
{
    if (CAN_GetITStatus(CAN1, CAN_IT_TME)) {           
        gpio_write_bit(GPIOE, LD7_COLOR_GREEN, Bit_SET);      
        CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
    }
}

void CAN1_RX1_IRQHandler()
{
    gpio_write_bit(GPIOE, LD4_COLOR_BLUE, Bit_SET);

    can_read(CAN1, CAN_FIFO1, &can_rx_msg);

    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);
}
