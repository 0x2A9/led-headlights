#ifndef LED_HEADLIGHTS_I2C_H
#define LED_HEADLIGHTS_I2C_H

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_i2c.h"
#include "stm32f30x_misc.h"
#include "stm32f30x_rcc.h"

typedef enum {
    I2C_MODE_IDLE,
    I2C_MODE_TX,
    I2C_MODE_RX
} i2c_mode_t;

extern uint8_t             *i2c_tx_buffer;
extern uint8_t             *i2c_rx_buffer;
extern volatile uint16_t   i2c_tx_size;
extern volatile uint16_t   i2c_rx_size;
extern volatile uint16_t   i2c_tx_index;
extern volatile uint16_t   i2c_rx_index;
extern volatile i2c_mode_t i2c_mode;

void i2c1_init(void);
void i2c_master_transmit_it(I2C_TypeDef* i2cx, uint8_t slave_addr, uint8_t *b_data, uint16_t size);
void i2c_master_receive_it(I2C_TypeDef* i2cx, uint8_t slave_addr, uint8_t *b_data, uint16_t size);
void i2c_master_receive_polling(I2C_TypeDef* i2cx, uint8_t device_address, uint8_t device_register, uint8_t *buffer, uint8_t size);
void i2c_master_transmit_polling(I2C_TypeDef* i2cx, uint8_t device_address, uint8_t device_register, uint8_t *buffer, uint8_t size);

#endif // LED_HEADLIGHTS_I2C_H
