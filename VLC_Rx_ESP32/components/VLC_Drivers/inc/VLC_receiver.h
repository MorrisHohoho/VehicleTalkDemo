#ifndef VLC_RECEIVER_H
#define VLC_RECEIVER_H

#include <stdint.h>


/**
 * The receiver state machine
 * TODO: Optimize it, the VLC_PRE state seems to be redundant.
*/
typedef enum{
    VLC_IDLE,
    VLC_PRE,
    VLC_RECEIVING
}RecvState;

/**
 * Used to indicate which receiver has received data.
*/
typedef enum{
    VLC_DATA_NONE,
    VLC_DATA_RX1,
    VLC_DATA_RX2,
    VLC_DATA_BOTH
}DataState;

/**
 * Init the receiver.
*/
void VLC_receiver_init();


/**
 * Monitor the receiver and fetches the data.
 * @param: rx1_buffer: The buffer that stores the data from rx1.
 * @param: rx2_buffer: The buffer that stores the data from rx2.
 * @return Return DataState to indicate which receiver has received data.
*/
DataState VLC_receiver_DoReceive(uint8_t* rx1_buffer,uint8_t* rx2_buffer);

#endif