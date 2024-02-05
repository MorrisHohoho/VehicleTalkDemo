//
// Created by Administrator on 12/1/2023.
//

#include "vehicle_servo.h"

#include "tim.h"

#define SERVO_TIM &htim14
#define SERVO_TIM_CH TIM_CHANNEL_1

#define SERVO_DUTY_0 (1389)
#define SERVO_DUTY_MINUS_45 (1500)
#define SERVO_DUTY_POSITIVE_45 (1294)

void vehicle_servo_init(){
    HAL_TIM_PWM_Start(SERVO_TIM,SERVO_TIM_CH);
    vehicle_servo_0();
}



void vehicle_servo_control(uint16_t Duty){
    __HAL_TIM_SetCompare(SERVO_TIM,SERVO_TIM_CH,Duty);
}

void vehicle_servo_0(){
    vehicle_servo_control(SERVO_DUTY_0);
}

void vehicle_servo_minus_45(){
    vehicle_servo_control(SERVO_DUTY_MINUS_45);
}

void vehicle_servo_positive_45(){
    vehicle_servo_control(SERVO_DUTY_POSITIVE_45);
}