#include "state_machine.h"

#define STATE_CNT (4u)
#define MAX_DUTY_CYCLE_TICK (4095u)
#define MIN_DUTY_CYCLE_TICK (0u)

typedef enum {
    PWM_CHAN_0,
    PWM_CHAN_1,
    PWM_CHAN_2,
    PWM_CHAN_3,
} pwm_chan_t;

static void run_state_1(void);
static void run_state_2(void);
static void run_state_3(void);
static void run_state_4(void);

hl_state_fn_t state_fns[STATE_CNT] = {
    [HL_STATE_1] = run_state_1,
    [HL_STATE_2] = run_state_2,
    [HL_STATE_3] = run_state_3,
    [HL_STATE_4] = run_state_4,
};

volatile hl_state_t current_state = HL_STATE_1;

static hl_state_t get_current_state(void);
static void set_current_state(hl_state_t m);
static hl_state_t detect_next_state(const uint8_t data[8]);
static hl_state_fn_t get_state_fn(hl_state_t m);

static hl_state_t get_current_state(void)
{
    return current_state;
}

static void set_current_state(hl_state_t m)
{
    current_state = m;
}

static hl_state_t detect_next_state(const uint8_t data[8])
{
    if (data[0] == HL_STATE_1) {
        return HL_STATE_1;
    } 
    
    if (data[0] == HL_STATE_2) {
        return HL_STATE_2;
    } 
    
    if (data[0] == HL_STATE_3) {
        return HL_STATE_3;
    } 

    if (data[0] == HL_STATE_4) {
        return HL_STATE_4;
    } 
    
    return current_state;
}

static hl_state_fn_t get_state_fn(hl_state_t m)
{
    return state_fns[m];
}

void state_machine_upd_state(const uint8_t data[8])
{
    set_current_state(detect_next_state(data));
}

void state_machine_run(void)
{
    get_state_fn(get_current_state())();
}

/**
 * Changes brightness of the all LEDs 
 * From the higs to low and vice versa
 */
static void run_state_1(void) 
{
    uint16_t i = 0;

    while (i < MAX_DUTY_CYCLE_TICK) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, i);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, i);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, i);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, i);

        i += 1;
    }

    while (i > MIN_DUTY_CYCLE_TICK) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, i);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, i);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, i);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, i);

        i -= 1;
    }
}

/**
 * Periodically turns all the LEDs on and off
 */
static void run_state_2(void) 
{
    uint16_t i = 0;
    
    while (i < 1500) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);

        i += 1;
    }

    while (i > 0) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);

        i -= 1;
    }
}

/**
 * Periodically turns two LEDs on and two off
 */
static void run_state_3(void) 
{
    uint16_t i = 0;
    
    while (i < 1500) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);

        i += 1;
    }

    while (i > 0) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);

        i -= 1;
    }
}

/**
 * Turns LEDs on one by one, turns all the LEDs off then
 */
static void run_state_4(void) 
{
    uint16_t i = 0;

    while (i < 375) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);

        i += 1;
    }
    
    while (i < 750) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);

        i += 1;
    }

    while (i < 1125) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);

        i += 1;
    }

    while (i < 1500) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MAX_DUTY_CYCLE_TICK);

        i += 1;
    }

    while (i > 0) {
        pca9685_set_pwm(PWM_CHAN_0, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_1, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_2, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);
        pca9685_set_pwm(PWM_CHAN_3, MIN_DUTY_CYCLE_TICK, MIN_DUTY_CYCLE_TICK);

        i -= 1;
    }
}
