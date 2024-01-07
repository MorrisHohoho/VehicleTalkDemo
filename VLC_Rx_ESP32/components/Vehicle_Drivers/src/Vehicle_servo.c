#include "Vehicle_servo.h"

#include "driver/ledc.h"
#include "driver/gpio.h"

#define VEHICLE_SERVO_PWM_DUTY_RESOLUTION LEDC_TIMER_12_BIT
#define VEHICLE_SERVO_PWM_FREQUENCY (50)

#define VEHICLE_SERVO_PWM_CHANNEL LEDC_CHANNEL_2
#define VEHICLE_SERVO_PWM_PIN GPIO_NUM_7


/**
 * Change the ms to real pwm duty
*/
static int helper_ms2real(double ms)
{
    return (int)((ms/20) * (1 << VEHICLE_SERVO_PWM_DUTY_RESOLUTION));
}

void Vehicle_servo_init()
{

    // 1. Setup the PWM
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE, // value must be LEDC_LOW_SPEED_MODE
        .duty_resolution = VEHICLE_SERVO_PWM_DUTY_RESOLUTION,
        .timer_num = LEDC_TIMER_1,
        .freq_hz = VEHICLE_SERVO_PWM_FREQUENCY, // Set output frequency at 50Hz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // 2. Setup the PWM GPIO
    int init_duty = helper_ms2real(1.5);
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = VEHICLE_SERVO_PWM_CHANNEL,
        .timer_sel = LEDC_TIMER_1,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = VEHICLE_SERVO_PWM_PIN,
        .duty = init_duty, // Set duty to 10%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void Vehicle_servo_change_angle(int angle)
{
    double ms = 0.011*angle+0.5;
    // printf("ms:%f\n",ms);
    int temp_duty = helper_ms2real(ms);
    // printf("temp_duty:%d\n",temp_duty);
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE,VEHICLE_SERVO_PWM_CHANNEL,temp_duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE,VEHICLE_SERVO_PWM_CHANNEL));
}