#ifndef __TX_TIMER_H_
#define __TX_TIMER_H_

#include <unistd.h>

/**
 * @brief Set up the timer, the resolution is 1MHz.
*/
void TX3_gpitimer_init();

/**
 * @brief Block us delay.
 * @param us: The intended delay time.
*/
void TX3_udelay(uint64_t us);

#endif