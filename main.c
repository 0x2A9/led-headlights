#include "stm32f30x.h"

#define PCA9685_ADDRESS              0x80
#define PCA9685_MODE1                0x0         // datasheet page no 10/52
#define PCA9685_PRE_SCALE            0xFE        // datasheet page no 13/52
#define PCA9685_LED0_ON_L            0x6         // datasheet page no 10/52
#define PCA9685_MODE1_SLEEP_BIT      4           // datasheet page no 14/52
#define PCA9685_MODE1_AI_BIT         5           // datasheet page no 14/52
#define PCA9685_MODE1_RESTART_BIT    7           // datasheet page no 14/52

uint8_t i2c_buffer[256];
void i2c_init();
void gpio_common_init();
void gpio_for_i2c_init();
void i2c_read(uint8_t device_address, uint8_t device_register, uint8_t *buffer, uint8_t size);
void i2c_write(uint8_t device_address, uint8_t device_register, uint8_t *buffer, uint8_t size);

int main(void)
{
    i2c_init();
    gpio_for_i2c_init();
    gpio_common_init();

    while (1) {
        i2c_read(PCA9685_ADDRESS, PCA9685_MODE1, i2c_buffer, 1);

        if (0 != i2c_buffer[0]) {
            GPIO_SetBits(GPIOE, GPIO_Pin_8);
        } else if (1 != i2c_buffer[0]) {
            GPIO_SetBits(GPIOE, GPIO_Pin_11);
        }
    }

    return 0;
}

void i2c_init()
{
    // Enable RCC clock for I2C1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    I2C_InitTypeDef I2C_InitStruct;

    I2C_InitStruct.I2C_Timing = 0x20B;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_OwnAddress1 = 0x0;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C1, &I2C_InitStruct);

    I2C_Cmd(I2C1, ENABLE);
}

/** 
  * I2C Pins configuration on the GPIO E port 
  * To only turn ON LEDs
  */
void gpio_common_init()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/** 
  * I2C pins configuration on the GPIO B port
  * To use I2C
  */ 
void gpio_for_i2c_init()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_4);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void i2c_read(uint8_t device_address, uint8_t device_register, uint8_t *buffer, uint8_t size) 
{
    // Check whether the I2C bus is idle
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_TransferHandling(I2C1, device_address, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

    // Waiting for TXDR register is empty
    // And the data to be transmitted may be written to TXDR
    while (I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);

    // Send slave register address the info is need to be read from
    I2C_SendData(I2C1, device_register);

    // Wait for the end of address transmission (EAT)
    // For master mode TC bit of the ISR register specifies EAT
    // It also read ISR register, so the TC bit is cleared
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET);

    // Initiate read operation
    I2C_TransferHandling(I2C1, device_address, size, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    for (int i = 0; i < size; ++i) {
        while (I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);

	buffer[i] = I2C_ReceiveData(I2C1);
    }

    while (I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);

    I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}

void i2c_write(uint8_t device_address, uint8_t device_register, uint8_t *buffer, uint8_t size)
{
    // Check whether the I2C bus is idle
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    I2C_TransferHandling(I2C1, device_address, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
    
    // Waiting for TXDR register is empty
    // And the data to be transmitted may be written to TXDR
    while (I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
    
    // Send slave register address the info is need to be written to
    I2C_SendData(I2C1, device_register);
    
    // Check that data have been transferred and RELOAD = 1
    while (I2C_GetFlagStatus(I2C1, I2C_ISR_TCR) == RESET);
    
    I2C_TransferHandling(I2C1, device_address, size, I2C_AutoEnd_Mode, I2C_No_StartStop);
    
    while (size) {
        while (I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
        
        // Write data to TXDR 
        I2C_SendData(I2C1, *buffer);
        
        buffer++;
        size--;
    }
    
    while (I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);

    I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}
