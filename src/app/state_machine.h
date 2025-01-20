#ifndef LED_HEADLIGHTS_STATE_MACHINE_H
#define LED_HEADLIGHTS_STATE_MACHINE_H

#include "drivers/pca9685.h"
#include "stm32f30x.h"

typedef enum {
    HL_STATE_1,
    HL_STATE_2,
    HL_STATE_3,
    HL_STATE_4,
} hl_state_t;

typedef void (*hl_state_fn_t)(void);

void state_machine_upd_state(const uint8_t data[8]);
void state_machine_run(void);

#endif // LED_HEADLIGHTS_STATE_MACHINE_H
