#include "drivers/gpio.h"
#include "drivers/can.h"
#include "drivers/exti.h"
#include "drivers/buttons.h"
#include "drivers/stm32f30x_it.h"
#include "stm32f30x.h"
#include "stm32f30x_can.h"

int main(void)
{
    gpio_init_leds();
    exti_init();
    can_init();

    while (1) {
        btn_reset_leds();
    }

    return 0;
}
