/**
 * Transmitter module for VLC defination.
*/

#ifndef __VLC_TRANSMITTER_H__
#define __VLC_TRANSMITTER_H__



/**
 * Used to identify which transmitter is used.
*/
typedef enum{
    VLC_TX1,
    VLC_TX2,
    VLC_TX_BOTH,
}TransmitterFlag;

/**
 * Init the transmitte with UART
*/
void VLC_transmitter_init();


/**
 * Send the data through VLC
 * @param data: The data will be sent. Must have '\0' as the end indicator.
 * @param flag: Which transmitter is used.
*/
void VLC_transmitter_DoSend(const char *data, TransmitterFlag flag);

/**
 * Transmit Idle signal 0XAA to maintain DC balance
 * @param flag: Which transmitter is used.
*/
void VLC_transmitter_DoIdle(TransmitterFlag flag);

#endif