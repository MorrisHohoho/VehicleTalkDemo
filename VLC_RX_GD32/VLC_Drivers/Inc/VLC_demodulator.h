//
// Created by Administrator on 11/30/2023.
//

#ifndef VLC_RX_GD32_VLC_DEMODULATOR_H
#define VLC_RX_GD32_VLC_DEMODULATOR_H

#include <stdint.h>

/**
 * Receive the data through VLC by UART.
 * @param buffer The data buffer that stores the data.
 */
void VLC_demodulator_receive(uint8_t* buffer);

#endif //VLC_RX_GD32_VLC_DEMODULATOR_H
