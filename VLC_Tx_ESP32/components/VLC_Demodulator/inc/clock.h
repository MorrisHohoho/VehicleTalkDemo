#ifndef __CLOCK_H_
#define __CLOCK_H_

#include  "driver/gptimer.h"
#include  "driver/gpio.h"

#include <string.h>

#define CLOCK_TIMES 16

static gptimer_handle_t gptimer_header = NULL;


static bool header_clock_state = false;
static bool header_detect_state =false;

esp_err_t header_clock_init(uint64_t duration,int rx_pin);
void header_clock_start();
void header_clock_stop();
bool header_clock_get_state();

bool get_header_detect_state();
void set_header_detect_state(bool state);

#endif