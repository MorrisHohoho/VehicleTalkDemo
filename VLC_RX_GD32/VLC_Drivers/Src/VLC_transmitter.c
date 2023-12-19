//
// Created by Administrator on 11/30/2023.
//

#include "../Inc/VLC_transmitter.h"
#include "../Inc/VLC_encoder.h"
#include "../Inc/VLC_timer.h"
#include "main.h"
#include "gpio.h"

#include "usart.h"
#include <string.h>


#define BIT_ADDR(byte_offset, bit_number) (volatile unsigned long*)(0x42000000+((byte_offset)<< 5) +(bit_number << 2))
#define GPIOA_OCTL_OFFSET ((GPIOA_BASE+0x14) - 0x40000000)

#define PAout(n) *(BIT_ADDR(GPIOA_OCTL_OFFSET,n))


void VLC_transmitter_send(const char *data) {
    int frame_counts = strlen(data) / PAYLOAD_LENGTH;
    if (strlen(data) % PAYLOAD_LENGTH != 0) {
        frame_counts++;
    }

    uint16_t duration = 1000000 / VLC_BUAD_RATE; //us
    duration *= 8;

    uint8_t tx_buf[FRAME_LENGTH * 2 + 2]; //payload after manchester encoding + 1 byte header + 1 byte tailer
    for (int i = 0; i < frame_counts; i++) {
        VLC_encode(data + i * PAYLOAD_LENGTH, i, tx_buf);

        //The tailer has to stop the UART transmitting
        HAL_UART_Transmit(&huart4, tx_buf, FRAME_LENGTH * 2 + 2, HAL_MAX_DELAY);    // Header + payload

        //Transmit the tailer: 0b11111111
        VLC_udelay(duration);
    }
}


void VLC_transmitter_idle() {
    uint8_t square_wave[] = {0xAA};
    for (int i = 0; i < 1000; i++) {
        HAL_UART_Transmit(&huart4, square_wave, 1, HAL_MAX_DELAY);
    }
}


void VLC_Gpio_transmitter_send(const char *data) {
//    int frame_counts = strlen(data) / PAYLOAD_LENGTH;
//    if (strlen(data) % PAYLOAD_LENGTH != 0) {
//        frame_counts++;
//    }
//
//    uint16_t duration = 1000000 / VLC_BUAD_RATE; //us
//    duration *= 8;
//
//    uint8_t tx_buf[FRAME_LENGTH * 2 + 2]; //payload after manchester encoding + 1 byte header + 1 byte tailer
//    for (int i = 0; i < frame_counts; i++) {
//        VLC_encode(data + i * PAYLOAD_LENGTH, i, tx_buf);
//
//        //The tailer has to stop the UART transmitting
//        HAL_UART_Transmit(&huart4, tx_buf, FRAME_LENGTH * 2 + 2, HAL_MAX_DELAY);    // Header + payload
//        for (int j = 0; j < FRAME_LENGTH * 2 + 2; j++) {
//            for (int b = 0; b < 8; b++) {
//
//            }
//        }
//
//        //Transmit the tailer: 0b11111111
//        VLC_udelay(duration);
//    }
    PAout(1) = 1;
    VLC_udelay(20);
    PAout(1) = 0;
    VLC_udelay(20);
}