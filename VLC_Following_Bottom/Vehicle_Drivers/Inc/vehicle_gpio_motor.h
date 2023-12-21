#ifndef VLC_TX_GD32_VEHICLE_MOTOR_H
#define VLC_TX_GD32_VEHICLE_MOTOR_H

/**
 * Motor control logic without velocity adjustment. Only gpio is used.
 */

#include <stdint.h>


void vehicle_gpio_motor_init();

void vehicle_gpio_motor_stop();

void vehicle_gpio_motor_forward();

void vehicle_gpio_motor_left();

void vehicle_gpio_motor_right();

void vehicle_gpio_motor_control(char state);


#endif