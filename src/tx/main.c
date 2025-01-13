#include "drivers/gpio.h"
#include "drivers/can.h"
#include "drivers/stm32f30x_it.h"
#include "stm32f30x.h"
#include "stm32f30x_can.h"

int main(void)
{
    gpio_output_mode_init();
    gpio_input_mode_init();
    can_init();

    can_tx_msg.StdId = 0;
    can_tx_msg.RTR = CAN_RTR_DATA;
    can_tx_msg.IDE = CAN_ID_EXT;
    can_tx_msg.DLC = 8;
    can_tx_msg.ExtId = 0x200;
    can_tx_msg.Data[7] = 1;
    can_tx_msg.Data[6] = 'N';
    can_tx_msg.Data[5] = 1;
    can_tx_msg.Data[4] = 1;
    can_tx_msg.Data[3] = 1;
    can_tx_msg.Data[2] = 0;
    can_tx_msg.Data[1] = 1;
    can_tx_msg.Data[0] = 1;

    BitAction button_state = Bit_RESET;

    while (1) {
        if (gpio_read_bit(GPIOD, GPIO_Pin_10) == Bit_SET) {
            gpio_write_bit(GPIOE, LD4_COLOR_BLUE, Bit_SET);
            
            if (Bit_RESET == button_state) {
                button_state = Bit_SET;

                if (can_write(CAN1, &can_tx_msg) == CAN_TxStatus_Failed) {
                    gpio_write_bit(GPIOE, LD3_COLOR_RED, Bit_SET);
                }
            }
        } else {
            button_state = Bit_RESET;
            gpio_write_bit(GPIOE, LD4_COLOR_BLUE | LD3_COLOR_RED | LD7_COLOR_GREEN, Bit_RESET);
        }
    }

    return 0;
}
