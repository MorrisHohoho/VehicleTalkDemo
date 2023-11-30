//
// Created by Administrator on 11/30/2023.
//

#include "VLC_demodulator.h"

#include "usart.h"
void VLC_demodulator_receive(uint8_t* buffer){
    HAL_UART_Receive_DMA(&huart4,buffer,RX_BUFFER_LENGTH);
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//    HAL_UART_Transmit(&huart1,(uint8_t*)"hello\n",6,0xFFFF);
//}