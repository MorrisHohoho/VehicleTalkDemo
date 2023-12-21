//
// Created by Administrator on 11/30/2023.
//

#ifndef VLC_TX_GD32_VLC_TIMER_H
#define VLC_TX_GD32_VLC_TIMER_H

#include <stdint.h>


/**
 * Init the timer, which will be used to usdelay.
 */
void VLC_timer_init();


/**
 * Using APB Timer to delay, in us.
 * @param us The delay time.
 */
void VLC_udelay(uint32_t us);


#endif //VLC_TX_GD32_VLC_TIMER_H
