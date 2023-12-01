//
// Created by Administrator on 12/1/2023.
//

#include "main.h"

#include "VLC_decoder.h"

void VLC_manchester_decoding(uint8_t *encoded_data, uint8_t *decoded_data) {
    int j = 0;

    // two bytes to one byte
    uint8_t BIT1 = 0b01000000;
    uint8_t BIT2 = 0b00010000;
    uint8_t BIT3 = 0b00000100;
    uint8_t BIT4 = 0b00000001;
    for (int i = 0; i < FRAME_LENGTH * 2; i += 2) {
        uint8_t VLC_temp = 0x00;
        uint8_t MSB = encoded_data[i];
        uint8_t LSB = encoded_data[i + 1];
        // MSB 4 bit
        VLC_temp = (MSB & BIT1) << 1; //MSB, 1;
        VLC_temp = VLC_temp | ((MSB & BIT2) << 2); //MSB, 2;
        VLC_temp = VLC_temp | ((MSB & BIT3) << 3); //MSB, 3;
        VLC_temp = VLC_temp | ((MSB & BIT4) << 4); //MSB, 4;

        // LSB 4 bit
        VLC_temp = VLC_temp | ((LSB & BIT1) >> 3); //LSB, 1;
        VLC_temp = VLC_temp | ((LSB & BIT2) >> 2); //LSB, 2;
        VLC_temp = VLC_temp | ((LSB & BIT3) >> 1); //LSB, 3;
        VLC_temp = VLC_temp | ((LSB & BIT4)); //LSB, 4;


        decoded_data[j++] = VLC_temp;
    }

}

void VLC_decode(uint8_t *encoded_data, uint8_t *decoded_data) {
    memset(decoded_data,0,FRAME_LENGTH);
    VLC_manchester_decoding(++encoded_data, decoded_data);
}