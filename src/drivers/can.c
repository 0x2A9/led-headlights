#include "can.h"

void can_init(void)
{
    /* 1. GPIO Config */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_9);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_9);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Pin   = GPIO_Pin_8; /* RX */
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    gpio.GPIO_Pin   = GPIO_Pin_9; /* TX */
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gpio);

    /* 2. CAN Config */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    CAN_InitTypeDef can;
    CAN_StructInit(&can); /* Reset CAN config */

    can.CAN_TTCM      = DISABLE;
    can.CAN_ABOM      = DISABLE;
    can.CAN_AWUM      = DISABLE;
    can.CAN_NART      = DISABLE;
    can.CAN_RFLM      = DISABLE;
    can.CAN_TXFP      = DISABLE;
    can.CAN_Mode      = CAN_Mode_Normal;
    can.CAN_SJW       = CAN_SJW_1tq; 
    can.CAN_BS1       = CAN_BS1_2tq; 
    can.CAN_BS2       = CAN_BS2_1tq; 
    can.CAN_Prescaler = 72;

    CAN_Init(CAN1, &can);

    /* 3. NVIC Config */
    NVIC_InitTypeDef nvic;

    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority        = 0;
    nvic.NVIC_IRQChannelCmd                = ENABLE;

#ifdef TX
    nvic.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn; /* TX ISR */
    CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  /* On mailbox 0|1|2 empty */
#else
    nvic.NVIC_IRQChannel = CAN1_RX1_IRQn; /* RX ISR */
    CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE); /* On FIFO 1 pending message */
#endif

    NVIC_Init(&nvic);
}

#ifdef RX

void can_init_filter(void)
{
    CAN_FilterInitTypeDef filter;

    filter.CAN_FilterNumber         = 1;
    filter.CAN_FilterActivation     = ENABLE;
    filter.CAN_FilterMode           = CAN_FilterMode_IdMask;
    filter.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
    filter.CAN_FilterIdHigh         = 0;
    filter.CAN_FilterIdLow          = 0;
    filter.CAN_FilterMaskIdHigh     = 0;
    filter.CAN_FilterMaskIdLow      = 0;
    filter.CAN_FilterScale          = CAN_FilterScale_16bit;

    CAN_FilterInit(&filter);
}

#endif

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
