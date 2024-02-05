/**
 * Created by Morris 11/30/2023
 * This file is the implementation of VLC_transmitter.
 * The MCU is GD32F450VKT6
 */

#include "../Inc/VLC_transmitter.h"
#include "../Inc/VLC_encoder.h"
#include "../Inc/VLC_timer.h"
#include "main.h"
#include "gpio.h"

#include "usart.h"
#include <string.h>

/**
 * The GPIO device register operation macro function.
 */
#define BIT_ADDR(byte_offset, bit_number) (volatile unsigned long*)(0x42000000+((byte_offset)<< 5) +(bit_number << 2))
#define GPIOA_OCTL_OFFSET ((GPIOA_BASE+0x14) - 0x40000000)

/**
 * Set the value of GPIO_A pins.
 * n: The gpio A pin.
 */
#define PAout(n) *(BIT_ADDR(GPIOA_OCTL_OFFSET,n))

/**
 * Get the n bit value of the num v.
 * v: The number v.
 * n: The intended bit.
 */
#define GET_BIT_OF(v,n) (((v)&(1<<(n)))?1:0)

/**
 * PA1: The IR LED transmitter controlled by the upper board.
 */
#define IR1 1

/**
 * PA2: The IR LED transmitter controlled by the upper board.
 */
#define IR2 2   //PA2



void VLC_UART_transmitter_send(const char *data) {
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


void VLC_UART_transmitter_idle() {
    uint8_t square_wave[] = {0xAA};
    for (int i = 0; i < 1000; i++) {
        HAL_UART_Transmit(&huart4, square_wave, 1, HAL_MAX_DELAY);
    }
}


void VLC_Gpio_transmitter_send(const char *data) {
    int frame_counts = strlen(data) / PAYLOAD_LENGTH;
    if (strlen(data) % PAYLOAD_LENGTH != 0) {
        frame_counts++;
    }

    PAout(IR1) = 0;
    PAout(IR2) = 0;
    uint8_t tx_buf[FRAME_LENGTH * 2 + 2]; //payload after manchester encoding + 1 byte header + 1 byte tailer
    for (int i = 0; i < frame_counts; i++) {
        VLC_encode(data + i * PAYLOAD_LENGTH, i, tx_buf);
        for (int j = 0; j < FRAME_LENGTH * 2 + 2; j++) {
            for (int b = 7; b>=0; b--) {
                PAout(IR1) = GET_BIT_OF(tx_buf[j],b);
                PAout(IR2) = GET_BIT_OF(tx_buf[j],b);
                VLC_udelay(VLC_BIT_DURATION);
            }
        }
    }
}

void VLC_Gpio_transmitter_idle(){
    for(int i=0;i<25000;i++)
    {
        HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);
        VLC_udelay(VLC_BIT_DURATION);
    }
}