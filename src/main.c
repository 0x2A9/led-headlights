#include "stm32f30x.h"
#include "drivers/i2c.h"
#include "drivers/gpio.h"

#define PCA9685_ADDRESS              0x80
#define PCA9685_MODE1                0x0         // datasheet page no 10/52
#define PCA9685_PRE_SCALE            0xFE        // datasheet page no 13/52
#define PCA9685_LED0_ON_L            0x6         // datasheet page no 10/52
#define PCA9685_MODE1_SLEEP_BIT      4           // datasheet page no 14/52
#define PCA9685_MODE1_AI_BIT         5           // datasheet page no 14/52
#define PCA9685_MODE1_RESTART_BIT    7           // datasheet page no 14/52

uint8_t i2c_buffer[256];

int main(void)
{
    i2c_init();
    gpio_for_i2c_init();
    gpio_init();

    while (1) {
        i2c_read(PCA9685_ADDRESS, PCA9685_MODE1, i2c_buffer, 1);

        if (0 != i2c_buffer[0]) {
            GPIO_SetBits(GPIOE, GPIO_Pin_8);
        } else if (1 != i2c_buffer[0]) {
            GPIO_SetBits(GPIOE, GPIO_Pin_11);
        }
    }

    return 0;
}
