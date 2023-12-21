//
// Created by Administrator on 11/30/2023.
//

#ifndef VLC_TX_GD32_VLC_ENCODER_H
#define VLC_TX_GD32_VLC_ENCODER_H

#include <stdint.h>


/**
 * Convert the "01" flow into manchester "01" flow.
 * i.e. : 1->01; 0->10
 * @param original_data Original data in bytes, i.e. "abcd"
 * @param encoded_data  Data after manchester encoding, i.e. 0b11110000->0b01010101,0b10101010.
 */
static void VLC_manchester_encode(const uint8_t* original_data, uint8_t* encoded_data);

/**
 * Convert the frame into the physical frame "01" flow.
 * @param data The manchester encoded data.
 * @param frame_num The frame number that indicates the order of the data.
 * @param tx_buf The fixed physical frame "01" flow that is transmitted.
 * i.e. Header[0xFF] + data + Tailer[0x00].
 */
void VLC_encode(const char* data, int frame_num, uint8_t* tx_buf);


#endif //VLC_TX_GD32_VLC_ENCODER_H
