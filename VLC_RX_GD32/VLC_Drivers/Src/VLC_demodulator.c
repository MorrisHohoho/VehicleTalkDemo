//
// Created by Administrator on 11/30/2023.
//

#include "VLC_demodulator.h"

#include "main.h"
#include "usart.h"

extern uint8_t DETECT_DATA_FLAG;
extern uint8_t data[FRAME_LENGTH];
extern uint8_t buffer[RX_BUFFER_LENGTH];

void VLC_demodulator_receive() {
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart4, buffer, RX_BUFFER_LENGTH);
}
