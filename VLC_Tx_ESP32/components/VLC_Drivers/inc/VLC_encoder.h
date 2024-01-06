#ifndef VLC_ENCODER_H
#define VLC_ENCODER_H

#include <stdint.h>

void VLC_encoder_DoEncode(const char* data, int frame_num, uint8_t* tx_buf);

#endif