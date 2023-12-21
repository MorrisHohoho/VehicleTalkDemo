//
// Created by Administrator on 11/30/2023.
//

#ifndef VLC_RX_GD32_VLC_RECEIVER_H
#define VLC_RX_GD32_VLC_RECEIVER_H

#include <stdint.h>

enum RECV_STATE{
    VLC_IDLE,
    VLC_PRE,
    VLC_RECEIVING
};

/**
 * Enable the receiver to receive the data through VLC by UART.
 */
void VLC_receiver_start();


/**
 * Decode the receiving data.
 * @param payload The array that stores the decoded data.
 */
void VLC_receiver_receive(uint8_t* payload);


/**
 * Receiving data from PD by using gpio.
 */
void VLC_Gpio_DoReceive();

#endif //VLC_RX_GD32_VLC_RECEIVER_H
