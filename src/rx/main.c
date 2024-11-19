#include "stm32f30x.h"
#include "drivers/i2c.h"
#include "drivers/gpio.h"
#include "drivers/pca9685.h"
#include "drivers/can.h"

int main(void)
{
    gpio_output_mode_init();
    gpio_i2c_mode_init();
    i2c1_init();
    gpio_can_mode_init();
    pca9685_init(PCA9685_DEFAULT_FREQUENCY);
    
    if (can_init() == CAN_InitStatus_Success) {
        gpio_write_bit(GPIOE, GPIO_Pin_11, Bit_SET);
    }
    can_filter_init();

    uint16_t counter = 0;
    uint16_t interanl_counter_max = 4095;

    while (1)  {
        CanRxMsg msg;

        if (can_read(CAN1, CAN_FIFO0, &msg) == 0) {
            gpio_write_bit(GPIOE, GPIO_Pin_8, Bit_SET);    
        }

        while (counter < interanl_counter_max)
        {
            pca9685_set_pwm(0, 0, counter);
            pca9685_set_pwm(1, 0, counter);
            pca9685_set_pwm(2, 0, counter);
            pca9685_set_pwm(3, 0, counter);

            counter += 1;
        }

        if (msg.Data[2] == 0) {
            gpio_write_bit(GPIOE, GPIO_Pin_10, Bit_SET);
        }

        while (counter > 0)
        {
            pca9685_set_pwm(0, 0, counter);
            pca9685_set_pwm(1, 0, counter);
            pca9685_set_pwm(2, 0, counter);
            pca9685_set_pwm(3, 0, counter);

            counter -= 1;
        }

        gpio_write_bit(GPIOE, GPIO_Pin_10, Bit_RESET);
    }

    return 0;
}
