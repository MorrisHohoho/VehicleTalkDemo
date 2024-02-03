//
// Created by Administrator on 12/2/2023.
//

#ifndef VLC_TX_GD32_VEHICLE_PWM_MOTOR_H
#define VLC_TX_GD32_VEHICLE_PWM_MOTOR_H

#include <stdint.h>


/**
 * Set the pwm duty to adjust the velocity of the motor.
 * @param duty The pwm duty from -100 to 100.
 * @param motor The motor adjusted, i.e., 'A' or 'B'.
 */
void vehicle_pwm_motor_set_velocity(int16_t duty, char motor);


/**
 * @brief Init the motor both motor A and motor B.
 */
void vehicle_pwm_motor_init();


/**
 * Stop the motor.
 * @param motor The motor adjusted, i.e., 'A' or 'B'.
 */
void vehicle_pwm_motor_stop(char motor);


/**
 * Forward the motor.
 * @param motor The motor adjusted, i.e., 'A' or 'B'.
 */
void vehicle_pwm_motor_forward(char motor);


/**
 * Backward the motor.
 * @param motor The motor adjusted, i.e., 'A' or 'B'.
 */
void vehicle_pwm_motor_backward(char motor);


/**
 * Change the vehicle status.
 * @param status The vehicle status and it should appeared in the decoded data 2nd byte, i.e., PSN+Status+Other data.
 * 'W': Forward.
 * 'S': Backward.
 * 'A': Turn left.
 * 'D': Turn right.
 * '0': Stop
 */
void vehicle_pwm_motor_control(char status);

#endif

