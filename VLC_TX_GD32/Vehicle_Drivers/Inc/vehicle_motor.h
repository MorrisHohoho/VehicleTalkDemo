//
// Created by Administrator on 12/2/2023.
//

#ifndef VLC_TX_GD32_VEHICLE_MOTOR_H
#define VLC_TX_GD32_VEHICLE_MOTOR_H

#include <stdint.h>


/**
 * Set the pwm duty to adjust the velocity of the motor.
 * @param Duty The pwm duty from 0 to 100.
 * @param motor The motor adjusted, i.e., 'A' or 'B'.
 */
void vehicle_motor_set_velocity(uint16_t Duty, char motor);


/**
 * @brief Init the motor both motor A and motor B.
 */
void vehicle_motor_init();


/**
 * Stop the motor.
 * @param motor The motor adjusted, i.e., 'A' or 'B'.
 */
void vehicle_motor_stop(char motor);


/**
 * Forward the motor.
 * @param motor The motor adjusted, i.e., 'A' or 'B'.
 */
void vehicle_motor_forward(char motor);


/**
 * Backward the motor.
 * @param motor The motor adjusted, i.e., 'A' or 'B'.
 */
void vehicle_motor_backward(char motor);


#endif //VLC_TX_GD32_VEHICLE_MOTOR_H
