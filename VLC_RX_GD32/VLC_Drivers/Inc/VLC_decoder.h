//
// Created by Administrator on 12/1/2023.
//

#ifndef VLC_RX_GD32_VLC_DECODER_H
#define VLC_RX_GD32_VLC_DECODER_H

#include<stdint.h>


/**
 * Decode the encoded data. Now only manchester encoding.
 * TODO: Add FEC to handle BER and PER, i.e. Spinal Code, LDPC or Raptor Code.
 * @param encoded_data The encoded data got from the receiver.
 * @param decoded_data The decoded data in byte.
 */
void VLC_decode(uint8_t* encoded_data, uint8_t* decoded_data);

#endif //VLC_RX_GD32_VLC_DECODER_H
