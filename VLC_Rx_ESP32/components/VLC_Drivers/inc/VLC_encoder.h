#ifndef __VLC_ENCODER_H__
#define __VLC_ENCODER_H__

#include <stdint.h>


/**
 * Encode the data and create the physical layer data packet. Only the manchester encoding is used currently.
 * Data packet structure: 0x1F (Header) + sequence number (1 byte) + data (VLC_PAYLOAD_LENGTH bytes) + 0XAA (Tail)
 * @param data: The original data.
 * @param frame_num: The data packet sequence number after segmentation.
 * @param tx_buf: The buffer used to store the encoded data.
*/
void VLC_encoder_DoEncode(const char* data, int frame_num, uint8_t* tx_buf);

#endif