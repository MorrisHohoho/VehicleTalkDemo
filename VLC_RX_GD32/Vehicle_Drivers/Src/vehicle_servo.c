////
//// Created by Administrator on 12/1/2023.
////
//
//#include "vehicle_servo.h"
//
//#include "tim.h"
//
//
//void vehicle_servo_init(){
//    HAL_TIM_Base_Start_IT(&htim1);
//    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);
//    vehicle_servo_0();
//}
//
//
//void vehicle_servo_control(uint16_t Duty){
//    uint16_t real_duty = Duty*(400-1);
//    __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,real_duty);
//}
//
//void vehicle_servo_0(){
//    vehicle_servo_control(7);
//}
//
//void vehicle_servo_minus_45(){
//    vehicle_servo_control(5);
//}
//
//void vehicle_servo_positive_45(){
//    vehicle_servo_control(10);
//}