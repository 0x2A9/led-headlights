#include "buttons.h"

#define BTN_OFFSET (8u)

static volatile uint8_t btn_prev_states = ((uint8_t)0x00);

int btn_is_set(btn_no_t b)
{
    return gpio_read_bit(GPIOD, b) == Bit_SET;
}

int btn_is_reset(btn_no_t b)
{
    return gpio_read_bit(GPIOD, b) == Bit_RESET;
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
