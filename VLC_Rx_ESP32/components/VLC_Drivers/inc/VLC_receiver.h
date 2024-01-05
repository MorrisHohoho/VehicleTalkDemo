#ifndef VLC_RECEIVER_H
#define VLC_RECEIVER_H

#include <stdint.h>


typedef enum{
    VLC_IDLE,
    VLC_PRE,
    VLC_RECEIVING
}RecvState;

typedef enum{
    VLC_DATA_NONE,
    VLC_DATA_RX1,
    VLC_DATA_RX2,
    VLC_DATA_BOTH
}DataState;

void VLC_receiver_init();

DataState VLC_receiver_DoReceive(uint8_t* rx1_buffer,uint8_t* rx2_buffer);


#endif