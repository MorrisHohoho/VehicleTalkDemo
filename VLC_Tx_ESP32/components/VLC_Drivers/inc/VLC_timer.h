#ifndef __VLC_TIMER_H__
#define __VLC_TIMER_H__

#include  "driver/gptimer.h"

#include <stdint.h>

#define VLC_DATA_HEDAER_HIGH_BITS_LENGTH 5
#define VLC_TIMER_RESOULUTION 1

void VLC_timer_PeriodElapsedCallback(gptimer_handle_t timer,void *arg);
esp_err_t VLC_timer_rx1N2_init(uint64_t count);

void VLC_timer_generalCallback(gptimer_handle_t timer,void *arg);
esp_err_t VLC_timer_general_init(uint64_t count, bool is_wallclock);
uint32_t get_VLC_general_timer_count();
#endif