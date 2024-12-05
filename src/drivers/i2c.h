#ifndef LED_HEADLIGHTS_I2C_H
#define LED_HEADLIGHTS_I2C_H

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_i2c.h"
#include "stm32f30x_rcc.h"

void i2c1_init();
void i2c_read(I2C_TypeDef* i2cx, uint8_t device_address, uint8_t device_register, uint8_t *buffer, uint8_t size);
void i2c_write(I2C_TypeDef* i2cx, uint8_t device_address, uint8_t device_register, uint8_t *buffer, uint8_t size);

#endif // LED_HEADLIGHTS_I2C_H
