#include "can.h"

CanTxMsg can_tx_msg;
CanRxMsg can_rx_msg;

void can_init()
{
    /* 1. GPIO Config */

    // 1.1 GPIO Clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_9);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_9);

    GPIO_InitTypeDef gpio;

    // 1.2 RX pin 
    gpio.GPIO_Pin = GPIO_Pin_8;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    // 1.3 TX Pin
    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gpio);

    /* 2. CAN Config */

    // 2.1 CAN Clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    CAN_InitTypeDef can;

    // 2.2 Reset CAN config
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);

    // 2.3 CAN init
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

    CAN_Init(CAN1, &can);

    /* 3. NVIC Config */

    NVIC_InitTypeDef nvic;

    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;

    // 3.1 Interrupts from receiver 
    nvic.NVIC_IRQChannel = CAN1_RX1_IRQn;
    NVIC_Init(&nvic);

    // 3.2 Interrupts from transmitter
    nvic.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;  
    NVIC_Init(&nvic);

    CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE); /* On FIFO 1 pending message */
    CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  /* On mailbox 0|1|2 empty */
}

void can_filter_init()
{
    CAN_FilterInitTypeDef filter;

    filter.CAN_FilterNumber = 1;
    filter.CAN_FilterActivation = ENABLE;
    filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    filter.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
    filter.CAN_FilterIdHigh = 0;
    filter.CAN_FilterIdLow = 0;
    filter.CAN_FilterMaskIdHigh = 0;
    filter.CAN_FilterMaskIdLow = 0;
    filter.CAN_FilterScale = CAN_FilterScale_16bit;

    CAN_FilterInit(&filter);
}

uint8_t can_read(CAN_TypeDef* canx, uint8_t fifo_number, CanRxMsg* msg)
{
    if (fifo_number == CAN_FIFO0) {
        if (CAN_GetFlagStatus(canx, CAN_FLAG_FMP0) != SET) {
            return 1;
        }
    } else {
        if (CAN_GetFlagStatus(canx, CAN_FLAG_FMP1) != SET) {
            return 1;
        }
    }

    CAN_Receive(canx, fifo_number, msg);
    CAN_FIFORelease(canx, fifo_number);

    return 0;
}

uint8_t can_write(CAN_TypeDef* canx, CanTxMsg* msg)
{
    uint8_t mb = 0;

    if ((mb = CAN_Transmit(canx, msg)) == CAN_NO_MB) {
        return CAN_TxStatus_Failed;
    }

    if (CAN_TransmitStatus(canx, mb) == CAN_TxStatus_Failed) {
        return CAN_TxStatus_Failed;
    }

    return CAN_TxStatus_Ok;
}
