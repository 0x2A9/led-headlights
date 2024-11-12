#include "pca9685.h"
#include "i2c.h"

void pca9685_init(uint16_t frequency)
{
    pca9685_set_pwm_frequency(frequency);
    pca9685_set_bit(PCA9685_MODE1, PCA9685_MODE1_AI_BIT, 1);
}

void pca9685_set_pwm(uint8_t channel, uint16_t on_time, uint16_t off_time)
{
    uint8_t register_address;
    uint8_t pwm[4];
    register_address = PCA9685_LED0_ON_L + (4 * channel);

    pwm[0] = on_time & 0xFF;
    pwm[1] = on_time >> 8;
    pwm[2] = off_time & 0xFF;
    pwm[3] = off_time >> 8;

    i2c_write(PCA9685_I2C, PCA9685_ADDRESS, register_address, pwm, 4);
}

void pca9685_set_pwm_frequency(uint16_t frequency)
{
    uint8_t prescale;

    if (frequency >= 1526) {
        prescale = 0x03;
    } else if (frequency <= 24) {
        prescale = 0xFF;
    } else {
        // Prescale changes 3 to 255 for 1526Hz to 24Hz
        prescale = 25000000 / (4096 * frequency);
    }

    pca9685_set_bit(PCA9685_MODE1, PCA9685_MODE1_SLEEP_BIT, 1);
    i2c_write(PCA9685_I2C, PCA9685_ADDRESS, PCA9685_PRE_SCALE, &prescale, 1);
    pca9685_set_bit(PCA9685_MODE1, PCA9685_MODE1_SLEEP_BIT, 0);
    pca9685_set_bit(PCA9685_MODE1, PCA9685_MODE1_RESTART_BIT, 1);
}

void pca9685_set_bit(uint8_t device_register, uint8_t bit, uint8_t value)
{
    uint8_t read_value;
    
    // Read all 8 bits and set only one bit to 0/1 and write all 8 bits back
    i2c_read(PCA9685_I2C, PCA9685_ADDRESS, device_register, &read_value, 1);

    if (value == 0) {
        read_value &= ~(1 << bit);
    } else {
        read_value |= (1 << bit);
    }

    i2c_write(PCA9685_I2C, PCA9685_ADDRESS, device_register, &read_value, 1);
}
