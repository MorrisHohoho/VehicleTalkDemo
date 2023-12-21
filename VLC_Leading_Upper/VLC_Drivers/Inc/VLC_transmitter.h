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
 * Sending square wave when no messages are sending.
 * TODO: Use Interrupt to stop the idle state.
 */
void VLC_transmitter_idle();


/**
 * Sending the data by using gpio.
 * @param data The original data without the PSN.
 */
void VLC_Gpio_transmitter_send(const char *data);


/**
 * Sending 1 second square wave signal.
 */
void VLC_Gpio_transmitter_idle();


#endif //VLC_TX_GD32_VLC_TRANSMITTER_H
