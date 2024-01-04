#ifndef __DECODER_H__
#define __DECODE_H__

#include <stdint.h>


//decode manchester
void decode_manchester(uint8_t *encode, uint8_t *decode, uint16_t length);

//decode ascii
void decode_ascii(const uint8_t *encoder, uint8_t *decode, uint16_t length);
#endif