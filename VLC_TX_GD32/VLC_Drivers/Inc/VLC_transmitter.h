//
// Created by Administrator on 11/30/2023.
//

#ifndef VLC_TX_GD32_VLC_TRANSMITTER_H
#define VLC_TX_GD32_VLC_TRANSMITTER_H


/**
 * Send the data through the VLC Transmitter.
 * @param data Data in chars, i.e. "abcdefg".
 */
void VLC_transmitter_send(const char* data);


/**
 * Sending idle signal when no messages are sending in order to DC balance.
 * TODO: Use Interrupt to stop the idle state.
 */
void VLC_transmitter_idle_signal();

#endif //VLC_TX_GD32_VLC_TRANSMITTER_H
