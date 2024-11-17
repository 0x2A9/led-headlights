#include "can.h"

uint8_t can_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_9);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_9);

    GPIO_InitTypeDef  gpio;

    /* Configure CAN pin: RX */
    gpio.GPIO_Pin = GPIO_Pin_8;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    /* Configure CAN pin: TX */
    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gpio);

    CAN_InitTypeDef can;

    /* CAN cell init */
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = DISABLE;
    can.CAN_Mode = CAN_Mode_Normal;

    can.CAN_SJW = CAN_SJW_1tq; 
    can.CAN_BS1 = CAN_BS1_2tq; 
    can.CAN_BS2 = CAN_BS2_1tq; 
    can.CAN_Prescaler = 72;

    return CAN_Init(CAN1, &can);
}

uint8_t can_write(CanTxMsg* msg)
{
    uint8_t mb = 0;

    if ((mb = CAN_Transmit(CAN1, msg)) == CAN_NO_MB) {
        return 1;
    }

    if (CAN_TransmitStatus(CAN1, mb) == CAN_TxStatus_Failed) {
        return 1;
    }

    while (CAN_TransmitStatus(CAN1, mb) == CAN_TxStatus_Pending);

    return 0;
}
