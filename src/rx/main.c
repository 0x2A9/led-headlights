#include "drivers/i2c.h"
#include "drivers/gpio.h"
#include "drivers/pca9685.h"
#include "drivers/can.h"
#include "drivers/stm32f30x_it.h"
#include "stm32f30x.h"

#include <string.h>

int main(void)
{
    gpio_output_mode_init();
    gpio_i2c_mode_init();
    i2c1_init();
    pca9685_init(PCA9685_DEFAULT_FREQUENCY);
    
    can_init(); 
    can_filter_init();

    uint16_t counter = 0;
    uint16_t interanl_counter_max = 4095;

    while (1)  {
        memset(&can_rx_msg.Data, Bit_RESET, sizeof(can_rx_msg.Data));

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

        if (can_rx_msg.Data[6] == 78) {
            gpio_write_bit(GPIOE, LD7_COLOR_GREEN, Bit_SET);
            pca9685_set_pwm(4, 0, 4095);
        } else {
            gpio_write_bit(GPIOE, LD4_COLOR_BLUE | LD7_COLOR_GREEN, Bit_RESET);
            pca9685_set_pwm(4, 0, 125);
        }
    }

    return 0;
}
