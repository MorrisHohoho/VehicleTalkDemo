//
// Created by Administrator on 12/1/2023.
//

#ifndef VLC_TX_GD32_VEHICLE_SERVO_H
#define VLC_TX_GD32_VEHICLE_SERVO_H

#include <stdint.h>


void vehicle_servo_init();



/**
 * Change the PWM duty
 * @param Duty From 1-20000. 1 = 1us.
 */
void vehicle_servo_control(uint16_t Duty);


/**
 * Turn the servo forward. 0.5ms/20ms
 */
void vehicle_servo_0();

/**
 * Turn the servo left 45 degrees. 1ms/20ms;
 */
void vehicle_servo_minus_45();

/**
 * Turn the servo right 45 degrees. 2ms/20ms;
 */
void vehicle_servo_positive_45();

#endif //VLC_TX_GD32_VEHICLE_SERVO_H
