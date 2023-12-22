//
// Created by Administrator on 11/30/2023.
//

#include "../Inc/VLC_timer.h"

#include "tim.h"


void VLC_timer_init(){
    __HAL_TIM_ENABLE(&htim3); // Enable the Timer, which is used by the VLC_timer
}

void VLC_udelay(uint32_t us) {
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    while ((__HAL_TIM_GET_COUNTER(&htim3)) < us);
}