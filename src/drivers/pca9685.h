#ifndef LED_HEADLIGHTS_PCA9685_H
#define LED_HEADLIGHTS_PCA9685_H

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"

#define PCA9685_ADDRESS             0x80
#define PCA9685_MODE1               0x0
#define PCA9685_PRE_SCALE           0xFE
#define PCA9685_LED0_ON_L           0x6
#define PCA9685_MODE1_SLEEP_BIT     0x4
#define PCA9685_MODE1_AI_BIT        0x5
#define PCA9685_MODE1_RESTART_BIT   0x7
#define PCA9685_DEFAULT_FREQUENCY   5000

#define PCA9685_I2C                 I2C1    // Default I2C device for the PCA9685

void pca9685_init(uint16_t frequency);
void pca9685_set_pwm(uint8_t channel, uint16_t on_time, uint16_t off_time);
void pca9685_set_pwm_frequency(uint16_t frequency);
void pca9685_set_bit(uint8_t device_register, uint8_t bit, uint8_t value);

#endif // LED_HEADLIGHTS_PCA9685_H