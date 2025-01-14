#include "drivers/gpio.h"
#include "drivers/can.h"
#include "drivers/exti.h"
#include "drivers/buttons.h"
#include "drivers/stm32f30x_it.h"
#include "stm32f30x.h"
#include "stm32f30x_can.h"

int main(void)
{
    gpio_output_mode_init();
    exti_init();
    can_init();

    while (1) {
        if (btn_is_reset(BTN_NO_1)) {
            btn_set_prev_state(BTN_NO_1, RESET);
            gpio_write_bit(GPIOE, LD9_COLOR_BLUE, Bit_RESET);
        }

        if (btn_is_reset(BTN_NO_2)) {
            btn_set_prev_state(BTN_NO_2, RESET);
            gpio_write_bit(GPIOE, LD8_COLOR_ORANGE, Bit_RESET);
        }

        if (btn_is_reset(BTN_NO_3)) {
            btn_set_prev_state(BTN_NO_3, RESET);
            gpio_write_bit(GPIOE, LD4_COLOR_BLUE, Bit_RESET);
        }

        if (btn_is_reset(BTN_NO_4)) {
            btn_set_prev_state(BTN_NO_4, RESET);
            gpio_write_bit(GPIOE, LD5_COLOR_ORANGE, Bit_RESET);
        }

        if (btn_is_reset(BTN_NO_1 | BTN_NO_2 | BTN_NO_3 | BTN_NO_4)) {
            btn_set_prev_state(BTN_NO_1 | BTN_NO_2 | BTN_NO_3 | BTN_NO_4, RESET);
            gpio_write_bit(GPIOE, LD7_COLOR_GREEN | LD3_COLOR_RED, Bit_RESET);
        }
    }

    return 0;
}
