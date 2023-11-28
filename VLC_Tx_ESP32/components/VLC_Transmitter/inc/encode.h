#ifndef __DECODER_H__
#define __DECODE_H__

#include <stdint.h>

/**
 * @brief Encapsulate the data in the fixed physical frame
 * @param data: char*, the original data.
 * @param frame_num: The physical frame number that indicates different 
 * data packets.
 * @param tx_buf: The physical frame in 01 flow.
*/
void encode(const char* data, int frame_num, uint8_t* tx_buf);

#endif