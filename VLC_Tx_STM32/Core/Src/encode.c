#include "encode.h"
#include "main.h"
#include <string.h>

void manchester_encode(const uint8_t* original_data, uint8_t* encoded_data)
{
    uint16_t tmp_16=0;
    uint8_t tmp_8 = 0;
    for(int i=0;i<FRAME_LEN;i++)
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

void encode(const char* data, int frame_num, uint8_t* tx_buf)
{
    int tx_buf_ptr = 0;
    int data_len =strlen(data);
    if(data_len>PAYLOAD_LEN)
    {
        data_len = PAYLOAD_LEN;
    }
    
    // 1. insert the header
    tx_buf[tx_buf_ptr++] = 0x00; //"00000000";

    // 2. encode the payload with manchester encoidng
    uint8_t encoded_data[FRAME_LEN*2];

    manchester_encode(data, encoded_data); //manchester encoding

    // copy the encoded data to the tx_buf
    for(int i=0;i<FRAME_LEN*2;i++)
    {
        tx_buf[tx_buf_ptr++] = encoded_data[i];
    }

    // 3. insert the tailer
    tx_buf[tx_buf_ptr++] = 0x55; //"01010101";
}
