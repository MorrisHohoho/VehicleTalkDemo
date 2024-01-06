#include "VLC_encoder.h"
#include "VLC_parameters.h"

#include <string.h>

#define VLC_FRAME_HEADER 0x1F
#define VLC_FRAME_TAILOR 0X00

static void VLC_encoder_manchester_encoding(const uint8_t* original_data, uint8_t* encoded_data)
{
    uint16_t tmp_16=0;
    uint8_t tmp_8 = 0;
    for(int i=0;i<VLC_FRAME_LENGTH;i++)
    {
        tmp_8 = original_data[i];
        for(int j=7;j>=0;j--)
        {
            tmp_16 <<= 2;
            tmp_16 |= ((tmp_8>>j)&0x01)?0b01:0b10; 
        }

        encoded_data[i*2]=(tmp_16>>8);
        encoded_data[i*2+1]=tmp_16&0x00FF;
    }
}

void VLC_encoder_DoEncode(const char* data, int frame_num, uint8_t* tx_buf){

    int tx_buf_ptr = 0;
    int data_len =strlen(data);
    if(data_len>VLC_PAYLOAD_LENGTH)
    {
        data_len = VLC_PAYLOAD_LENGTH;
    }
    
    // 1. insert the header
    tx_buf[tx_buf_ptr++] = VLC_FRAME_HEADER;

    // 2. encode the payload with manchester encoding
    uint8_t original_data[VLC_FRAME_LENGTH];
    uint8_t encoded_data[VLC_FRAME_LENGTH*2];

    // init the frame.
    for(int i=0;i<VLC_FRAME_LENGTH;i++)
    {
        original_data[i] = 0;
    }

    original_data[0]=frame_num;  // the first byte is the frame number

    // copy the data, if the data is not enough, fill the rest with 0
    for(int i=0;i<data_len;i++)
    {
        original_data[i+1] = data[i];
    }

    VLC_encoder_manchester_encoding(original_data, encoded_data); //manchester encoding

    // copy the encoded data to the tx_buf
    for(int i=0;i<VLC_FRAME_LENGTH*2;i++)
    {
        tx_buf[tx_buf_ptr++] = encoded_data[i];
    }

    // 3. insert the tailer
    tx_buf[tx_buf_ptr++] = VLC_FRAME_TAILOR;
}