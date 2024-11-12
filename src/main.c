#include "stm32f30x.h"
#include "drivers/i2c.h"
#include "drivers/gpio.h"
#include "drivers/pca9685.h"

int main(void)
{
    i2c1_init();

    gpio_for_i2c1_init();
    gpio_init();
    
    pca9685_init(PCA9685_DEFAULT_FREQUENCY);

    uint16_t counter = 0;
    uint16_t interanl_counter_max = 4095;

    while (1)  {
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

        GPIO_SetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_11);
    }

    return 0;
}
