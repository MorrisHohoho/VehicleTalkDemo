#ifndef _ENCODE_H_
#define _ENCODE_H_


#include <stdint.h>

void manchester_encode(const uint8_t* original_data, uint8_t* encoded_data);

void encode(const char* data, int frame_num, uint8_t* tx_buf);
#endif