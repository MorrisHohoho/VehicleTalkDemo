#include "decoder.h"
#include <stdio.h>
void decode_manchester(uint8_t *VLC_encode, uint8_t *decode, uint16_t length)
{
    int j = 0;
    //    for(int i=0;i<20;i++)
    //     {
    //         printf("%x ",VLC_encode[i]);
    //     }
    //     printf("\n");
    // two bytes to one byte
    uint8_t BIT1=0b01000000;
    uint8_t BIT2=0b00010000;
    uint8_t BIT3=0b00000100;
    uint8_t BIT4=0b00000001;
    for(int i=0;i<length;i+=2)
    {
        uint8_t VLC_temp = 0x00;
        uint8_t MSB = VLC_encode[i];
        uint8_t LSB = VLC_encode[i+1];
        // MSB 4 bit
        VLC_temp = (MSB&BIT1)<<1; //MSB, 1;
        VLC_temp = VLC_temp | ((MSB&BIT2)<<2); //MSB, 2;
        VLC_temp = VLC_temp | ((MSB&BIT3)<<3); //MSB, 3;
        VLC_temp = VLC_temp | ((MSB&BIT4)<<4); //MSB, 4;

        // LSB 4 bit
        VLC_temp = VLC_temp | ((LSB&BIT1)>>3); //LSB, 1;
        VLC_temp = VLC_temp | ((LSB&BIT2)>>2); //LSB, 2;
        VLC_temp = VLC_temp | ((LSB&BIT3)>>1); //LSB, 3;
        VLC_temp = VLC_temp | ((LSB&BIT4)); //LSB, 4;


        decode[j++] = VLC_temp;
    }
}

void decode_ascii(const uint8_t *VLC_encode, uint8_t *decode, uint16_t length)
{
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (VLC_encode[i * 8 + j] == 0)
            {
                decode[i] = decode[i] << 1;
            }
            else
            {
                decode[i] = decode[i] << 1;
                decode[i] = decode[i] | 0x01;
            }
        }
    }
}