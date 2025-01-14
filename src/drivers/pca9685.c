#include "pca9685.h"

void pca9685_init(uint16_t frequency)
{
    pca9685_set_pwm_frequency(frequency);
    pca9685_set_bit(PCA9685_MODE1, PCA9685_MODE1_AI_BIT, 1);
}

pca9685_status_t pca9685_set_pwm_frequency(uint16_t frequency)
{
    uint8_t prescale = 0x7F;    /* Default fallback */

    if (frequency >= 1526) {
        prescale = 0x03; 
    } else if (frequency <= 24) {
        prescale = 0xFF;
    } else {
        /* Prescale changes 3 to 255 for 1526Hz to 24Hz */
        unsigned long calc = 25000000UL / (4096UL * (unsigned long)frequency);
        
        prescale = (uint8_t)calc;
    }


    if (PCA9685_STATUS_ERROR == pca9685_set_bit(PCA9685_MODE1, PCA9685_MODE1_SLEEP_BIT, 1)) {
        return PCA9685_STATUS_ERROR;
    }

    if (PCA9685_STATUS_ERROR == pca9685_write_reg(PCA9685_PRE_SCALE, &prescale, 1)) {
        return PCA9685_STATUS_ERROR;
    }

    if (PCA9685_STATUS_ERROR == pca9685_set_bit(PCA9685_MODE1, PCA9685_MODE1_SLEEP_BIT, 0)) {
        return PCA9685_STATUS_ERROR;
    }


    if (PCA9685_STATUS_ERROR == pca9685_set_bit(PCA9685_MODE1, PCA9685_MODE1_RESTART_BIT, 1)) {
        return PCA9685_STATUS_ERROR;
    }

    return PCA9685_STATUS_OK;
}

pca9685_status_t pca9685_set_bit(uint8_t device_register, 
                                 uint8_t bit, 
                                 uint8_t value)
{
    uint8_t reg_val;

    if (PCA9685_STATUS_ERROR == pca9685_read_reg(device_register, &reg_val, 1)) {
        return PCA9685_STATUS_ERROR;
    }

    if (value) {
        reg_val |= (uint8_t)(1 << bit);
    } else {
        reg_val &= (uint8_t)~(1 << bit);
    }

    return pca9685_write_reg(device_register, &reg_val, 1);
}

pca9685_status_t pca9685_set_pwm(uint8_t channel, 
                                 uint16_t on_time, 
                                 uint16_t off_time)
{
    uint8_t reg_addr = (uint8_t)(PCA9685_LED0_ON_L + (4 * channel));
    uint8_t pwm_data[4];

    pwm_data[0] = (uint8_t)(on_time & 0xFF);
    pwm_data[1] = (uint8_t)((on_time >> 8) & 0xFF);
    pwm_data[2] = (uint8_t)(off_time & 0xFF);
    pwm_data[3] = (uint8_t)((off_time >> 8) & 0xFF);

    return pca9685_write_reg(reg_addr, pwm_data, 4);
}

pca9685_status_t pca9685_write_reg(uint8_t reg_addr, uint8_t *b_data, uint16_t size)
{
    if ((size + 1) > PCA9685_I2C_BUFFER_SIZE) {
        return PCA9685_STATUS_ERROR;
    }

    uint8_t txB[16];

    memset(txB, 0, sizeof(txB));
    txB[0] = reg_addr;
    memcpy(&txB[1], b_data, size);

    i2c_master_transmit_it(PCA9685_I2C, PCA9685_ADDR_7BIT, txB, (uint16_t)(size + 1));

    if (PCA9685_STATUS_ERROR == pca9685_i2c_wait_util_idle()) {
        return PCA9685_STATUS_ERROR;
    }

    return PCA9685_STATUS_OK;
}

pca9685_status_t pca9685_read_reg(uint8_t reg_addr, uint8_t *b_data, uint16_t size)
{
    if (size == 0) {
        return PCA9685_STATUS_OK;
    }

    i2c_master_transmit_it(PCA9685_I2C, PCA9685_ADDR_7BIT, &reg_addr, 1);
    if (PCA9685_STATUS_ERROR == pca9685_i2c_wait_util_idle()) {
        return PCA9685_STATUS_ERROR;
    }

    i2c_master_receive_it(PCA9685_I2C, PCA9685_ADDR_7BIT, b_data, size);
    if (PCA9685_STATUS_ERROR == pca9685_i2c_wait_util_idle()) {
        return PCA9685_STATUS_ERROR;
    }
    
    return PCA9685_STATUS_OK;
}

pca9685_status_t pca9685_i2c_wait_util_idle(void)
{
    uint32_t timeout = PCA9685_I2C_TIMEOUT;
    
    while ((i2c_mode != I2C_MODE_IDLE) && (--timeout > 0)) {
        /* Wait for I2C to free its state */
    }

    if (timeout == 0) {
        return PCA9685_STATUS_ERROR;
    }

    return PCA9685_STATUS_OK;
}
