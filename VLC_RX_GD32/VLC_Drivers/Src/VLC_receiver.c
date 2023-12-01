//
// Created by Administrator on 11/30/2023.
//

#include "VLC_receiver.h"
#include "VLC_decoder.h"

#include "main.h"
#include "usart.h"

extern uint8_t DETECT_DATA_FLAG;
extern uint8_t data[FRAME_LENGTH * 2+2];
extern uint8_t buffer[RX_BUFFER_LENGTH];

void VLC_receiver_start() {
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart4, buffer, RX_BUFFER_LENGTH);
}

void VLC_receiver_receive(uint8_t *payload) {
        /** The data packet is wrong!**/
        if (data[0] != 0xFF) {
            payload[0] = 0xFF;
        } else {
            VLC_decode(data,payload);
        }
}
