#include "drivers/i2c.h"
#include "drivers/gpio.h"
#include "drivers/pca9685.h"
#include "drivers/can.h"
#include "drivers/stm32f30x_it.h"
#include "app/state_machine.h"
#include "stm32f30x.h"

int main(void)
{
    gpio_init_leds();
    i2c1_init();
    pca9685_init(PCA9685_DEFAULT_FREQUENCY);
    
    can_init(); 
    can_init_filter();

    while (1)  {
        state_machine_run();        
        gpio_write_bit(GPIOE, LD7_COLOR_GREEN, RESET);
    }

    return 0;
}
