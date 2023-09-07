#ifndef __DEMODULATOR_H__
#define __DEMODULATOR_H__

#include "clock.h"

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"

#define ESP_INTR_FLAG_DEFAULT 0
#define FRAME_IN_BYTES 119 //118 PAYLOAD + 1  Protection Byte

void demodulator_config(const int tx_pin, const int rx_pin,const int baud_rate);

void demodulator_receive(uint8_t *buffer);

/**
 * @brief the isr function for detecting the header, 
*/
static void IRAM_ATTR gpio_isr_handler_tim(void *ard);


#endif