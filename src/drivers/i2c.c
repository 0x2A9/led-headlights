#include "i2c.h"

uint8_t             *i2c_tx_buffer = 0;
uint8_t             *i2c_rx_buffer = 0;
volatile uint16_t   i2c_tx_size    = 0;
volatile uint16_t   i2c_rx_size    = 0;
volatile uint16_t   i2c_tx_index   = 0;
volatile uint16_t   i2c_rx_index   = 0;
volatile i2c_mode_t i2c_mode       = I2C_MODE_IDLE;

void i2c1_init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);     /* Enable GPIOB Clock */

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);     /* Reset gpio */

    gpio.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_OD;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOB, &gpio);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_4);    /* I2C1 SCL */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_4);    /* I2C1 SDA */

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    I2C_InitTypeDef i2c;
    I2C_StructInit(&i2c);   /* Reset i2c */

    i2c.I2C_Timing              = 0x20E;
    i2c.I2C_Mode                = I2C_Mode_I2C;
    i2c.I2C_OwnAddress1         = 0x00;
    i2c.I2C_Ack                 = I2C_Ack_Enable;
    i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C1, &i2c);
    I2C_Cmd(I2C1, ENABLE);

    I2C_ITConfig(I2C1,
                 I2C_IT_ERRI | I2C_IT_TCI | I2C_IT_STOPI |
                 I2C_IT_NACKI | I2C_IT_ADDRI | I2C_IT_RXI | I2C_IT_TXI,
                 ENABLE);

    NVIC_InitTypeDef nvic;

    nvic.NVIC_IRQChannel                   = I2C1_EV_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority        = 0;
    nvic.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic);

    nvic.NVIC_IRQChannel                   = I2C1_ER_IRQn;
    NVIC_Init(&nvic);
}

void i2c_master_transmit_it(I2C_TypeDef* i2cx, 
                            uint8_t slave_addr, 
                            uint8_t *b_data, 
                            uint16_t size)
{
    if (0 == size) {
        return;
    }

    i2c_tx_buffer = b_data;
    i2c_tx_size   = size;
    i2c_tx_index  = 0;
    i2c_mode      = I2C_MODE_TX;

    uint16_t address_bits = (uint16_t)(slave_addr << 1);

    I2C_TransferHandling(i2cx,
                         address_bits,
                         size,
                         I2C_AutoEnd_Mode,
                         I2C_Generate_Start_Write);
}

void i2c_master_receive_it(I2C_TypeDef* i2cx, 
                           uint8_t slave_addr, 
                           uint8_t *b_data, 
                           uint16_t size)
{
    if (0 == size) {
        return;
    }

    i2c_rx_buffer = b_data;
    i2c_rx_size   = size;
    i2c_rx_index  = 0;
    i2c_mode      = I2C_MODE_RX;

    uint16_t address_bits = (uint16_t)(slave_addr << 1);

    I2C_TransferHandling(i2cx,
                         address_bits,
                         size,
                         I2C_AutoEnd_Mode,
                         I2C_Generate_Start_Read);
}

void i2c_master_receive_polling(I2C_TypeDef* i2cx, 
                                uint8_t device_address, 
                                uint8_t device_register, 
                                uint8_t *buffer, 
                                uint8_t size)
{
    // Check whether the I2C bus is idle
    while (I2C_GetFlagStatus(i2cx, I2C_FLAG_BUSY));

    I2C_TransferHandling(i2cx, device_address, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

    // Waiting for TXDR register is empty
    // And the data to be transmitted may be written to TXDR
    while (I2C_GetFlagStatus(i2cx, I2C_ISR_TXIS) == RESET);

    // Send slave register address the info is need to be read from
    I2C_SendData(i2cx, device_register);

    // Wait for the end of address transmission (EAT)
    // For master mode TC bit of the ISR register specifies EAT
    // It also read ISR register, so the TC bit is cleared
    while (I2C_GetFlagStatus(i2cx, I2C_FLAG_TC) == RESET);

    // Initiate read operation
    I2C_TransferHandling(i2cx, device_address, size, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    for (int i = 0; i < size; ++i) {
        while (I2C_GetFlagStatus(i2cx, I2C_ISR_RXNE) == RESET);

        buffer[i] = I2C_ReceiveData(i2cx);
    }

    while (I2C_GetFlagStatus(i2cx, I2C_ISR_STOPF) == RESET);

    I2C_ClearFlag(i2cx, I2C_ICR_STOPCF);
}

void i2c_master_transmit_polling(I2C_TypeDef* i2cx, 
                                 uint8_t device_address, 
                                 uint8_t device_register, 
                                 uint8_t *buffer, 
                                 uint8_t size)
{
    // Check whether the I2C bus is idle
    while (I2C_GetFlagStatus(i2cx, I2C_FLAG_BUSY));

    I2C_TransferHandling(i2cx, device_address, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

    // Waiting for TXDR register is empty
    // And the data to be transmitted may be written to TXDR
    while (I2C_GetFlagStatus(i2cx, I2C_ISR_TXIS) == RESET);

    // Send slave register address the info is need to be written to
    I2C_SendData(i2cx, device_register);

    // Check that data have been transferred and RELOAD = 1
    while (I2C_GetFlagStatus(i2cx, I2C_ISR_TCR) == RESET);

    I2C_TransferHandling(i2cx, device_address, size, I2C_AutoEnd_Mode, I2C_No_StartStop);

    while (size) {
        while (I2C_GetFlagStatus(i2cx, I2C_ISR_TXIS) == RESET);

        // Write data to TXDR
        I2C_SendData(i2cx, *buffer);

        buffer++;
        size--;
    }

    while (I2C_GetFlagStatus(i2cx, I2C_ISR_STOPF) == RESET);

    I2C_ClearFlag(i2cx, I2C_ICR_STOPCF);
}
