//
// Created by Administrator on 11/30/2023.
//

#ifndef VLC_RX_GD32_VLC_RECEIVER_H
#define VLC_RX_GD32_VLC_RECEIVER_H

#include <stdint.h>

/**
 * Receive the data through VLC by UART.
 */
void VLC_receiver_start();


/**
 * Decode the receiving data.
 * @param payload The array that stores the decoded data.
 */
void VLC_receiver_receive(uint8_t* payload);

#endif //VLC_RX_GD32_VLC_RECEIVER_H
