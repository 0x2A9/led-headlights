#ifndef LED_HEADLIGHTS_BUTTONS_H
#define LED_HEADLIGHTS_BUTTONS_H

#include "drivers/gpio.h"
#include "stm32f30x_gpio.h"

typedef enum {
    BTN_NO_1 = GPIO_Pin_8,
    BTN_NO_2 = GPIO_Pin_9,
    BTN_NO_3 = GPIO_Pin_10,
    BTN_NO_4 = GPIO_Pin_11,
} btn_no_t;

typedef enum {
    HL_MODE_1,
    HL_MODE_2,
    HL_MODE_3,
    HL_MODE_4,
} hl_mode_t;

int btn_is_set(btn_no_t b);
int btn_is_reset(btn_no_t b);
void btn_set_prev_state(btn_no_t b, FlagStatus s);
FlagStatus btn_get_prev_state(btn_no_t b);

#endif // LED_HEADLIGHTS_BUTTONS_H
