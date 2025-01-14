#ifndef LED_HEADLIGHTS_PCA9685_H
#define LED_HEADLIGHTS_PCA9685_H

#include "i2c.h"
#include <string.h>

#define PCA9685_ADDR_7BIT         0x40    /* PCA9685 I2C address */
#define PCA9685_MODE1             0x00
#define PCA9685_MODE2             0x01
#define PCA9685_LED0_ON_L         0x06    /* PWM turn on register */
#define PCA9685_PRE_SCALE         0xFE
#define PCA9685_DEFAULT_FREQUENCY 0x32    /* 50 Hz */

#define PCA9685_MODE1_RESTART_BIT 0x7
#define PCA9685_MODE1_SLEEP_BIT   0x4
#define PCA9685_MODE1_AI_BIT      0x5

#define PCA9685_I2C_TIMEOUT       (1000000UL)

#define PCA9685_I2C_BUFFER_SIZE   0x10

#define PCA9685_I2C               I2C1

typedef enum {
    PCA9685_STATUS_OK,
    PCA9685_STATUS_ERROR
} pca9685_status_t;

void pca9685_init(uint16_t frequency);

pca9685_status_t pca9685_set_pwm_frequency(uint16_t frequency);
pca9685_status_t pca9685_set_bit(uint8_t device_register, 
                                 uint8_t bit, 
                                 uint8_t value);
pca9685_status_t pca9685_set_pwm(uint8_t channel, 
                                 uint16_t on_time, 
                                 uint16_t off_time);

pca9685_status_t pca9685_read_reg(uint8_t reg_addr, 
                                  uint8_t *b_data, 
                                  uint16_t size);
pca9685_status_t pca9685_write_reg(uint8_t reg_addr, 
                                   uint8_t *b_data, 
                                   uint16_t size);

pca9685_status_t pca9685_i2c_wait_util_idle(void);

#endif // LED_HEADLIGHTS_PCA9685_H
