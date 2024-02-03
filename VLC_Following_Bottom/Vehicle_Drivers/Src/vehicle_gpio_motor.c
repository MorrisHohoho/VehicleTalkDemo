#include "vehicle_gpio_motor.h"
#include "vehicle_servo.h"

#include "gpio.h"
#include "tim.h"

#include "main.h"


void vehicle_gpio_motor_init() {
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
}


void vehicle_gpio_motor_stop(){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
}



void vehicle_gpio_motor_forward(){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,2000);
}

void vehicle_gpio_motor_left(){
    vehicle_gpio_motor_stop();
    vehicle_servo_minus_45();
    vehicle_gpio_motor_forward();
}

void vehicle_gpio_motor_right(){
    vehicle_gpio_motor_stop();
    vehicle_servo_positive_45();
    vehicle_gpio_motor_forward();
}

void vehicle_gpio_motor_control(char state){
    switch (state) {
        case 'W': {
            vehicle_servo_0();
            vehicle_gpio_motor_forward();
            break;
        }
        case 'S': {
            //do something;
            break;
        }
        case 'A': {
            vehicle_gpio_motor_left();
            break;
        }
        case 'D': {
            vehicle_gpio_motor_right();
            break;
        }
        case 'X': {
            vehicle_servo_0();
            vehicle_gpio_motor_stop();
            break;
        }
    }
}
