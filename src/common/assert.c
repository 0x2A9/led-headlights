#include "common/assert.h"

void assert_handler(void) {
    const uint32_t counter_max = 1000000;
    volatile uint32_t counter = 0;          /* GCC performs unwanted optimization */

    gpio_write_bit(GPIOE, LD_COLOR_ALL, RESET);

    while (1)  {
        gpio_write_bit(GPIOE, LD3_COLOR_RED | LD10_COLOR_RED, SET);

        while (counter < counter_max) {
            counter += 1;
        }

        gpio_write_bit(GPIOE, LD3_COLOR_RED | LD10_COLOR_RED, RESET);

        while (counter > 0) {
            counter -= 1;
        }
    }
}
