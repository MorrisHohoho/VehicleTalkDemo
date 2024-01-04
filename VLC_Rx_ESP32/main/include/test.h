#ifndef __TEST_H__
#define __TEST_H__

#include "clock.h"


#include "demodulator.h"
#include "decoder.h"
#include "freertos/FreeRTOS.h"
#include <stdio.h>


/**
 * @brief print all the OOK signals to the screen
 */
void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length);

/**
 * @brief get the original frame
 */
void test_get_orignal_frame(uint8_t* buffer);


#endif