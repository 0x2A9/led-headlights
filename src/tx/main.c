#include "stm32f30x.h"
#include "stm32f30x_can.h"
#include "drivers/gpio.h"
#include "drivers/can.h"

CanTxMsg msg;

int main(void)
{
    gpio_output_mode_init();
    gpio_input_mode_init();
    gpio_can_mode_init();

    if (can_init() == CAN_InitStatus_Success) {
        gpio_write_bit(GPIOE, GPIO_Pin_10, Bit_SET);
    }

    msg.StdId = 0;
    msg.RTR = CAN_RTR_DATA;
    msg.IDE = CAN_ID_EXT;
    msg.DLC = 8;
    msg.ExtId = 0x200;
    msg.Data[7] = 1;
    msg.Data[6] = 'N';
    msg.Data[5] = 1;
    msg.Data[4] = 1;
    msg.Data[3] = 1;
    msg.Data[2] = 0;
    msg.Data[1] = 1;
    msg.Data[0] = 1;

    while (1) {
        if (can_write(&msg) == 0) {
            gpio_write_bit(GPIOE, GPIO_Pin_11, Bit_SET);
        }
    }

    return 0;
}
