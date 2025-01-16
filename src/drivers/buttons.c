#include "buttons.h"

#define BTN_OFFSET (8u)

static volatile uint8_t btn_prev_states = ((uint8_t)0x00);

int btn_is_set(btn_no_t b)
{
    return gpio_read_bit(GPIOD, b) == SET;
}

int btn_is_reset(btn_no_t b)
{
    return gpio_read_bit(GPIOD, b) == RESET;
}

void btn_set_prev_state(btn_no_t b, FlagStatus s)
{
    if (SET == s) {
        btn_prev_states |= ((uint8_t)(b >> BTN_OFFSET));
    } else {
        btn_prev_states &= ~((uint8_t)(b >> BTN_OFFSET));
    }
}

FlagStatus btn_get_prev_state(btn_no_t b)
{
    return btn_prev_states & ((uint8_t)(b >> BTN_OFFSET))
        ? SET 
        : RESET;
}

void btn_reset_leds(void)
{
    if (btn_is_reset(BTN_NO_1)) {
        btn_set_prev_state(BTN_NO_1, RESET);
        gpio_write_bit(GPIOE, LD9_COLOR_BLUE, RESET);
    }

    if (btn_is_reset(BTN_NO_2)) {
        btn_set_prev_state(BTN_NO_2, RESET);
        gpio_write_bit(GPIOE, LD8_COLOR_ORANGE, RESET);
    }

    if (btn_is_reset(BTN_NO_3)) {
        btn_set_prev_state(BTN_NO_3, RESET);
        gpio_write_bit(GPIOE, LD4_COLOR_BLUE, RESET);
    }

    if (btn_is_reset(BTN_NO_4)) {
        btn_set_prev_state(BTN_NO_4, RESET);
        gpio_write_bit(GPIOE, LD5_COLOR_ORANGE, RESET);
    }

    if (btn_is_reset(BTN_NO_1 | BTN_NO_2 | BTN_NO_3 | BTN_NO_4)) {
        btn_set_prev_state(BTN_NO_1 | BTN_NO_2 | BTN_NO_3 | BTN_NO_4, RESET);
        gpio_write_bit(GPIOE, LD7_COLOR_GREEN | LD3_COLOR_RED, RESET);
    }
}
