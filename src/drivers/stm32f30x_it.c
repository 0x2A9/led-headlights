#include "stm32f30x_it.h"

#ifdef TX
void USB_HP_CAN1_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_TME)) {           
        gpio_write_bit(GPIOE, LD7_COLOR_GREEN, SET);      
        CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line8) == SET) {
        for (int i = 0; i < 0x7FFFF; i++); /* Skip bouncing, wait idle state */

        if (btn_is_set(BTN_NO_1)) {
            gpio_write_bit(GPIOE, LD9_COLOR_BLUE, SET);

            if (btn_get_prev_state(BTN_NO_1) == RESET) {
                btn_set_prev_state(BTN_NO_1, SET);

                CanTxMsg msg = {
                    .DLC     = 1,
                    .Data[0] = HL_STATE_1,
                };

                if (can_write(CAN1, &msg) == CAN_TxStatus_Failed) {
                    gpio_write_bit(GPIOE, LD3_COLOR_RED, SET);
                }
            } 
        }    
    
        EXTI_ClearITPendingBit(EXTI_Line8);
    }   

    if (EXTI_GetITStatus(EXTI_Line9) == SET) {
        for (int i = 0; i < 0x7FFFF; i++); /* Skip bouncing, wait idle state */

        if (btn_is_set(BTN_NO_2)) {
            gpio_write_bit(GPIOE, LD8_COLOR_ORANGE, SET);

            if (btn_get_prev_state(BTN_NO_2) == RESET) {
                btn_set_prev_state(BTN_NO_2, SET);

                CanTxMsg msg = {
                    .DLC     = 1,
                    .Data[0] = HL_STATE_2,
                };

                if (can_write(CAN1, &msg) == CAN_TxStatus_Failed) {
                    gpio_write_bit(GPIOE, LD3_COLOR_RED, SET);
                }
            }  
        }    
    
        EXTI_ClearITPendingBit(EXTI_Line9);
    } 
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line10) == SET) {
        for (int i = 0; i < 0x7FFFF; i++); /* Skip bouncing, wait idle state */

        if (btn_is_set(BTN_NO_3)) { 
            gpio_write_bit(GPIOE, LD4_COLOR_BLUE, SET);

            if (btn_get_prev_state(BTN_NO_3) == RESET) {
                btn_set_prev_state(BTN_NO_3, SET);

                CanTxMsg msg = {
                    .DLC     = 1,
                    .Data[0] = HL_STATE_3,
                };

                if (can_write(CAN1, &msg) == CAN_TxStatus_Failed) {
                    gpio_write_bit(GPIOE, LD3_COLOR_RED, SET);
                }
            }   
        } 
    
        EXTI_ClearITPendingBit(EXTI_Line10);
    } 

    if (EXTI_GetITStatus(EXTI_Line11) == SET) {
        for (int i = 0; i < 0x7FFFF; i++); /* Skip bouncing, wait idle state */

        if (btn_is_set(BTN_NO_4)) {
            gpio_write_bit(GPIOE, LD5_COLOR_ORANGE, SET);

            if (btn_get_prev_state(BTN_NO_4) == RESET) {
                btn_set_prev_state(BTN_NO_4, SET);

                CanTxMsg msg = {
                    .DLC     = 1,
                    .Data[0] = HL_STATE_4,
                };

                if (can_write(CAN1, &msg) == CAN_TxStatus_Failed) {
                    gpio_write_bit(GPIOE, LD3_COLOR_RED, SET);
                }
            }  
        }    
    
        EXTI_ClearITPendingBit(EXTI_Line11);
    }   
}
#else
void CAN1_RX1_IRQHandler(void)
{
    gpio_write_bit(GPIOE, LD7_COLOR_GREEN, SET);

    CanRxMsg msg;
    memset(&msg.Data, RESET, sizeof(msg.Data));

    can_read(CAN1, CAN_FIFO1, &msg);

    state_machine_upd_state(msg.Data);

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
#endif
