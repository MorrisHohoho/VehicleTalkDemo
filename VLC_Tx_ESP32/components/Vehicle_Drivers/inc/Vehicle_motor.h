#ifndef __VEHICLE__MOTOR__H__
#define __VEHICLE__MOTOR__H__



void Vehicle_motor_init();

/**
 * Change the motor speed.
 * @param duty: double value ranges from 0~1
*/
void Vehicle_Rmotor_change_speed(double duty);
/**
 * Change the motor speed.
 * @param duty: double value ranges from 0~1
*/
void Vehicle_Lmotor_change_speed(double duty);

void Vehicle_Rmotor_stop();
void Vehicle_Lmotor_stop();

void Vehicle_Rmotor_forward();
void Vehicle_Rmotor_backward();

void Vehicle_Lmotor_forward();
void Vehicle_Lmotor_backward();

void Vehicle_motor_control(char state);


#endif