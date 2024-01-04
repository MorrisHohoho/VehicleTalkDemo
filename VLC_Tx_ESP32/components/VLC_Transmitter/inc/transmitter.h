#ifndef __DEMODULATOR_H__
#define __DEMODULATOR_H__


#include "encode.h"

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#include <string.h>

#define ESP_INTR_FLAG_DEFAULT 0


/**
 * @param tx_pin: The transmitter gpio pin;
 * @param baud_rate: The transmitting baud rate.
 * @param frame_length: The length of the fixed physical frame, in bytes.
*/
void VLC_transmitter_init(const uint16_t tx_pin, const int baud_rate, const uint16_t data_length);


/**
 * @param data: The sending data, in chars.
*/
void VLC_transmitter_send(const char* data);

#endif