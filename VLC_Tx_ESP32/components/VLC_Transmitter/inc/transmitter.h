#ifndef __DEMODULATOR_H__
#define __DEMODULATOR_H__


#include "encode.h"
#include "clock.h"

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"

#include <string.h>

#define ESP_INTR_FLAG_DEFAULT 0

void VLC_transmitter_init(const int tx_pin,const int baud_rate,const int frame_length);


void VLC_transmitter_send(const char* data);

#endif