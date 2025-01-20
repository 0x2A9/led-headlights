#ifndef LED_HEADLIGHTS_ASSERT_H
#define LED_HEADLIGHTS_ASSERT_H

#include "drivers/gpio.h"
#include "stm32f30x.h"

#ifdef USE_ASSERTS
    #define ASSERT(expression)          \
        do {                            \
            if (!(expression)) {        \
                assert_handler();       \
            }                           \
        } while (0);
#else
    #define ASSERT(expression) ((void)0U)
#endif

void assert_handler(void);

#endif // LED_HEADLIGHTS_ASSERT_H
