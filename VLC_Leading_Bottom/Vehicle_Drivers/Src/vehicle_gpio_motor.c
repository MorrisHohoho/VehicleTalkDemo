#include "vehicle_gpio_motor.h"
#include "vehicle_servo.h"

#include "gpio.h"


void vehicle_gpio_motor_init() {
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
}

void vehicle_gpio_motor_stop(){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
}

void vehicle_gpio_motor_forward(){
    vehicle_servo_0();
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
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
            vehicle_gpio_motor_stop();
            break;
        }
    }
}
