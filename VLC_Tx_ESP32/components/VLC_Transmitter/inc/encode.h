#ifndef __DECODER_H__
#define __DECODE_H__

#include <stdint.h>


#define PAYLOAD_LEN 59
#define FRAME_LEN (PAYLOAD_LEN+1)

void manchester_encode(const uint8_t* original_data, uint8_t* encoded_data);

void encode(const char* data, int frame_num, uint8_t* tx_buf);

#endif