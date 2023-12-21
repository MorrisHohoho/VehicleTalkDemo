////
//// Created by Administrator on 12/2/2023.
////
//
//
//#include "vehicle_servo.h"
//#include "vehicle_motor.h"
//
//#include"gpio.h"
//#include"tim.h"
//
//
//#define MOTORA_PWM &htim14
//#define MOTORA_PWM_CHANNEL TIM_CHANNEL_1
//#define MOTORB_PWM &htim8
//#define MOTORB_PWM_CHANNEL TIM_CHANNEL_1
//
//
//void vehicle_motor_set_velocity(uint16_t Duty, char motor){
//    uint16_t real_duty = Duty*(10-1);
//    switch (motor) {
//        case 'A':{
//            __HAL_TIM_SetCompare(MOTORA_PWM,MOTORA_PWM_CHANNEL,real_duty);
//            break;
//        }
//        case 'B':{
//            __HAL_TIM_SetCompare(MOTORB_PWM,MOTORB_PWM_CHANNEL,real_duty);
//        }
//    }
//}
//
//void vehicle_motor_init(){
//    HAL_TIM_PWM_Start(MOTORA_PWM,MOTORA_PWM_CHANNEL);
//    vehicle_motor_set_velocity(50,'A');
//    vehicle_motor_stop('A');
//
//    HAL_TIM_Base_Start_IT(MOTORB_PWM);
//    HAL_TIMEx_PWMN_Start(MOTORB_PWM,MOTORB_PWM_CHANNEL);
//    vehicle_motor_set_velocity(50,'B');
//    vehicle_motor_stop('B');
//}
//
//void vehicle_motor_stop(char motor){
//    switch (motor) {
//        case 'A':{
//            HAL_GPIO_WritePin(MOTORA_POSITIVE_GPIO_Port,MOTORA_POSITIVE_Pin,0);
//            HAL_GPIO_WritePin(MOTORA_NEGATIVE_GPIO_Port,MOTORA_NEGATIVE_Pin,0);
//            break;
//        }
//        case 'B':{
//            HAL_GPIO_WritePin(MOTORB_POSITIVE_GPIO_Port,MOTORB_POSITIVE_Pin,0);
//            HAL_GPIO_WritePin(MOTORB_NEGATIVE_GPIO_Port,MOTORB_NEGATIVE_Pin,0);
//        }
//    }
//}
//
//void vehicle_motor_forward(char motor){
//    switch (motor) {
//        case 'A':{
//            HAL_GPIO_WritePin(MOTORA_POSITIVE_GPIO_Port,MOTORA_POSITIVE_Pin,1);
//            HAL_GPIO_WritePin(MOTORA_NEGATIVE_GPIO_Port,MOTORA_NEGATIVE_Pin,0);
//            break;
//        }
//        case 'B':{
//            HAL_GPIO_WritePin(MOTORB_POSITIVE_GPIO_Port,MOTORB_POSITIVE_Pin,1);
//            HAL_GPIO_WritePin(MOTORB_NEGATIVE_GPIO_Port,MOTORB_NEGATIVE_Pin,0);
//        }
//    }
//}
//
//void vehicle_motor_backward(char motor){
//    switch (motor) {
//        case 'A':{
//            HAL_GPIO_WritePin(MOTORA_POSITIVE_GPIO_Port,MOTORA_POSITIVE_Pin,0);
//            HAL_GPIO_WritePin(MOTORA_NEGATIVE_GPIO_Port,MOTORA_NEGATIVE_Pin,1);
//            break;
//        }
//        case 'B':{
//            HAL_GPIO_WritePin(MOTORB_POSITIVE_GPIO_Port,MOTORB_POSITIVE_Pin,0);
//            HAL_GPIO_WritePin(MOTORB_NEGATIVE_GPIO_Port,MOTORB_NEGATIVE_Pin,1);
//        }
//    }
//}
//
//void vehicle_motor_control(char status){
//    switch (status) {
//        case 'W':{
//            vehicle_motor_stop('A');
//            vehicle_motor_stop('B');
//            vehicle_servo_0();
//            vehicle_motor_forward('A');
//            vehicle_motor_forward('B');
//            break;
//        }
//        case 'S':{
//            vehicle_motor_stop('A');
//            vehicle_motor_stop('B');
//            vehicle_servo_0();
//            vehicle_motor_backward('A');
//            vehicle_motor_backward('B');
//            break;
//        }
//        case 'A':{
//            vehicle_motor_stop('A');
//            vehicle_motor_stop('B');
//            vehicle_servo_minus_45();
//            vehicle_motor_forward('A');
//            vehicle_motor_forward('B');
//            break;
//        }
//        case 'D':{
//            vehicle_motor_stop('A');
//            vehicle_motor_stop('B');
//            vehicle_servo_positive_45();
//            vehicle_motor_forward('A');
//            vehicle_motor_forward('B');
//            break;
//        }
//        case '0':{
//            vehicle_motor_stop('A');
//            vehicle_motor_stop('B');
//            vehicle_servo_0();
//            break;
//        }
//    }
//}
//
