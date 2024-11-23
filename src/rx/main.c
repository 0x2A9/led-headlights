#include "stm32f30x.h"
#include "drivers/i2c.h"
#include "drivers/gpio.h"
#include "drivers/pca9685.h"
#include "drivers/can.h"
#include <string.h>

int main(void)
{
    gpio_output_mode_init();
    gpio_i2c_mode_init();
    i2c1_init();
    gpio_can_mode_init();
    pca9685_init(PCA9685_DEFAULT_FREQUENCY);
    
    if (can_init() == CAN_InitStatus_Success) {
        gpio_write_bit(GPIOE, GPIO_Pin_10, Bit_SET);
    }
    
    can_filter_init();

    uint16_t counter = 0;
    uint16_t interanl_counter_max = 4095;
    CanRxMsg msg;

    while (1)  {
        memset(&msg.Data, Bit_RESET, sizeof(msg.Data));

        if (can_read(CAN1, CAN_FIFO0, &msg) == 0) {
            gpio_write_bit(GPIOE, GPIO_Pin_8, Bit_SET);    
        } else {
            gpio_write_bit(GPIOE, GPIO_Pin_8, Bit_RESET);
        }

        while (counter < interanl_counter_max)
        {
            pca9685_set_pwm(0, 0, counter);
            pca9685_set_pwm(1, 0, counter);
            pca9685_set_pwm(2, 0, counter);
            pca9685_set_pwm(3, 0, counter);

            counter += 1;
        }

        while (counter > 0)
        {
            pca9685_set_pwm(0, 0, counter);
            pca9685_set_pwm(1, 0, counter);
            pca9685_set_pwm(2, 0, counter);
            pca9685_set_pwm(3, 0, counter);

            counter -= 1;
        }

        if (msg.Data[6] == 78) {
            gpio_write_bit(GPIOE, GPIO_Pin_11, Bit_SET);
            pca9685_set_pwm(4, 0, 4095);
        } else {
            gpio_write_bit(GPIOE, GPIO_Pin_11, Bit_RESET);
            pca9685_set_pwm(4, 0, 125);
        }
    }

    return 0;
}
