#ifndef __DEMODULATOR_H__
#define __DEMODULATOR_H__

#include "clock.h"

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0
#define FRAME_IN_BYTES 120 //118 PAYLOAD + 1  Protection Byte


void VLC_demodulator_config_debugging(const int tx_pin, const int rx_pin,const int baud_rate);

void VLC_demodulator_config(const uint16_t rx_pin, const int baud_rate, const uint16_t frame_length);

void demodulator_receive(uint8_t *buffer);

/**
 * @brief the isr function for detecting the header, 
*/
static void IRAM_ATTR gpio_isr_handler_tim(void *ard);


#endif