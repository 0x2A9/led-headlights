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

void I2C1_EV_IRQHandler(void)
{
    /* Transmit interrupt */
    if (I2C_GetITStatus(I2C1, I2C_IT_TXIS) == SET) {
        if (i2c_mode == I2C_MODE_TX && i2c_tx_index < i2c_tx_size) {
            I2C_SendData(I2C1, i2c_tx_buffer[i2c_tx_index++]);
        }
    }

    /* Receive interrupt */
    if (I2C_GetITStatus(I2C1, I2C_IT_RXNE) == SET) {
        if (i2c_mode == I2C_MODE_RX && i2c_rx_index < i2c_rx_size) {
            i2c_rx_buffer[i2c_rx_index++] = I2C_ReceiveData(I2C1);
        }
    }

    /* Transmit completed interrupt */
    if (I2C_GetITStatus(I2C1, I2C_IT_TC) == SET) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_TC);
    }

    /* Stop signal interrupt */
    if (I2C_GetITStatus(I2C1, I2C_IT_STOPF) == SET) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_STOPF);
        i2c_mode = I2C_MODE_IDLE;
    }

    /* Address matched for slave mode */
    if (I2C_GetITStatus(I2C1, I2C_IT_ADDR) == SET) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_ADDR);
    }

    /* Not aknowledged message */
    if (I2C_GetITStatus(I2C1, I2C_IT_NACKF) == SET) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_NACKF);
        i2c_mode = I2C_MODE_IDLE;
    }
}

void I2C1_ER_IRQHandler(void)
{
    /* Bus error */
    if (I2C_GetITStatus(I2C1, I2C_IT_BERR) == SET) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
        i2c_mode = I2C_MODE_IDLE;
    }

    /* Arbitration lost */
    if (I2C_GetITStatus(I2C1, I2C_IT_ARLO) == SET) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_ARLO);
        i2c_mode = I2C_MODE_IDLE;
    }

    /* Overrun */
    if (I2C_GetITStatus(I2C1, I2C_IT_OVR) == SET) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_OVR);
        i2c_mode = I2C_MODE_IDLE;
    }
}
