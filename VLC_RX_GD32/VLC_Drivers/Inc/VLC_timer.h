//
// Created by Administrator on 11/30/2023.
//

#ifndef VLC_TX_GD32_VLC_TIMER_H
#define VLC_TX_GD32_VLC_TIMER_H

#include <stdint.h>


/**
 * Using APB Timer to delay, in us.
 * @param us The delay time.
 */
void VLC_udelay(uint32_t us);


#endif //VLC_TX_GD32_VLC_TIMER_H
