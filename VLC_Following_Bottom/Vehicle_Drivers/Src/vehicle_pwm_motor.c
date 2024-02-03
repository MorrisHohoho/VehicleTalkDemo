//
// Created by Administrator on 12/2/2023.
//


#include "vehicle_servo.h"
#include "vehicle_pwm_motor.h"

#include"gpio.h"
#include"tim.h"
#include "main.h"

#define PWM_FREQ 10000
#define PWM_DUTY_MUL_FACTOR (PWM_FREQ/100)


void vehicle_pwm_motor_set_velocity(int16_t duty, char motor) {
    int isBack = 0;
    uint16_t real_duty = 0;
    if (duty >= 0) {
        real_duty = duty * PWM_DUTY_MUL_FACTOR;
    } else {
        duty = 0 - duty;
        real_duty = duty * PWM_DUTY_MUL_FACTOR;
        isBack = 1;
    }
    switch (motor) {
        case 'A': {
            if (isBack == 0) {
                        __HAL_TIM_SetCompare(VEHICLE_LEFT_MOTOR_PWMA_TIM,
                                             VEHICLE_LEFT_MOTOR_PWMA_CHANNEL,
                                             real_duty);
                        __HAL_TIM_SetCompare(VEHICLE_LEFT_MOTOR_PWMB_TIM,
                                             VEHICLE_LEFT_MOTOR_PWMB_CHANNEL,
                                             0);
            } else {
                        __HAL_TIM_SetCompare(VEHICLE_LEFT_MOTOR_PWMA_TIM,
                                             VEHICLE_LEFT_MOTOR_PWMA_CHANNEL,
                                             0);
                        __HAL_TIM_SetCompare(VEHICLE_LEFT_MOTOR_PWMB_TIM,
                                             VEHICLE_LEFT_MOTOR_PWMB_CHANNEL,
                                             real_duty);
            }
            break;
        }
        case 'B': {
            if (isBack == 0) {
                        __HAL_TIM_SetCompare(VEHICLE_RIGHT_MOTOR_PWMA_TIM,
                                             VEHICLE_RIGHT_MOTOR_PWMA_CHANNEL,
                                             real_duty);
                        __HAL_TIM_SetCompare(VEHICLE_RIGHT_MOTOR_PWMB_TIMx,
                                             VEHICLE_RIGHT_MOTOR_PWMB_CHANNELN,
                                             0);
            } else {
                        __HAL_TIM_SetCompare(VEHICLE_RIGHT_MOTOR_PWMA_TIM,
                                             VEHICLE_RIGHT_MOTOR_PWMA_CHANNEL,
                                             0);
                        __HAL_TIM_SetCompare(VEHICLE_RIGHT_MOTOR_PWMB_TIMx,
                                             VEHICLE_RIGHT_MOTOR_PWMB_CHANNELN,
                                             real_duty);
            }
            break;
        }
    }
}

void vehicle_pwm_motor_init() {
    HAL_TIM_PWM_Start(VEHICLE_LEFT_MOTOR_PWMA_TIM, VEHICLE_LEFT_MOTOR_PWMA_CHANNEL);
    vehicle_pwm_motor_stop('A');

    HAL_TIMEx_PWMN_Start(VEHICLE_RIGHT_MOTOR_PWMA_TIM, VEHICLE_RIGHT_MOTOR_PWMA_CHANNEL);
    vehicle_pwm_motor_stop('B');
}

void vehicle_pwm_motor_stop(char motor) {
    switch (motor) {
        case 'A': {
            vehicle_pwm_motor_set_velocity(0, 'A');
            break;
        }
        case 'B': {
            vehicle_pwm_motor_set_velocity(0, 'B');
            break;
        }
    }
}

void vehicle_pwm_motor_forward(char motor) {
    switch (motor) {
        case 'A': {
            vehicle_pwm_motor_set_velocity(20, 'A');
            break;
        }
        case 'B': {
            vehicle_pwm_motor_set_velocity(20, 'B');
            break;
        }
    }
}

void vehicle_pwm_motor_backward(char motor) {
    switch (motor) {
        case 'A': {
            vehicle_pwm_motor_set_velocity(-20, 'A');
            break;
        }
        case 'B': {
            vehicle_pwm_motor_set_velocity(-20, 'B');
            break;
        }
    }
}

void vehicle_pwm_motor_control(char status) {
    switch (status) {
        case 'W': {
            vehicle_pwm_motor_stop('A');
            vehicle_pwm_motor_stop('B');
            vehicle_servo_0();
            vehicle_pwm_motor_forward('A');
            vehicle_pwm_motor_forward('B');
            break;
        }
        case 'S': {
            vehicle_pwm_motor_stop('A');
            vehicle_pwm_motor_stop('B');
            vehicle_servo_0();
            vehicle_pwm_motor_backward('A');
            vehicle_pwm_motor_backward('B');
            break;
        }
        case 'A': {
            vehicle_pwm_motor_stop('A');
            vehicle_pwm_motor_stop('B');
            vehicle_servo_minus_45();
            vehicle_pwm_motor_forward('A');
            vehicle_pwm_motor_forward('B');
            break;
        }
        case 'D': {
            vehicle_pwm_motor_stop('A');
            vehicle_pwm_motor_stop('B');
            vehicle_servo_positive_45();
            vehicle_pwm_motor_forward('A');
            vehicle_pwm_motor_forward('B');
            break;
        }
        case 'X': {
            vehicle_pwm_motor_stop('A');
            vehicle_pwm_motor_stop('B');
            vehicle_servo_0();
            break;
        }
    }
}

