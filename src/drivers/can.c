#include "can.h"

uint8_t can_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

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

void can_filter_init()
{
    CAN_FilterInitTypeDef filter;

    filter.CAN_FilterNumber = 0;
    filter.CAN_FilterActivation = ENABLE;
    filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    filter.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
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
