#include "Vehicle_motor.h"

#include "driver/ledc.h"
#include "driver/gpio.h"

#define VEHICLE_MOTOR_PWM_DUTY_RESOLUTION LEDC_TIMER_10_BIT
#define VEHICLE_MOTOR_PWM_FREQUENCY (10000) // The PWM frequency is 10 KHz.

#define VEHICLE_RMOTOR_PWM_CHANNEL LEDC_CHANNEL_0
#define VEHICLE_LMOTOR_PWM_CHANNEL LEDC_CHANNEL_1

#define VEHICLE_RMOTOR_PWM_PIN GPIO_NUM_15
#define VEHICLE_LMOTOR_PWM_PIN GPIO_NUM_16

#define VEHICLE_RMOTOR_POS_PIN GPIO_NUM_18
#define VEHICLE_RMOTOR_NEG_PIN GPIO_NUM_11

#define VEHICLE_LMOTOR_POS_PIN GPIO_NUM_10
#define VEHICLE_LMOTOR_NEG_PIN GPIO_NUM_17

static int helper_duty2real(double duty)
{
    return (int)(duty * (1 << VEHICLE_MOTOR_PWM_DUTY_RESOLUTION));
}

void Vehicle_motor_init()
{

    // 1. Setup the PWM
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE, // value must be LEDC_LOW_SPEED_MODE
        .duty_resolution = VEHICLE_MOTOR_PWM_DUTY_RESOLUTION,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = VEHICLE_MOTOR_PWM_FREQUENCY, // Set output frequency at 10 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // 2. Setup the PWM GPIO
    int init_duty = helper_duty2real(0.3);
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = VEHICLE_RMOTOR_PWM_CHANNEL,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = VEHICLE_RMOTOR_PWM_PIN,
        .duty = init_duty, // Set duty to 10%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    ledc_channel.gpio_num = VEHICLE_LMOTOR_PWM_PIN;
    ledc_channel.channel = VEHICLE_LMOTOR_PWM_CHANNEL;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // 3. Setup the Control GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << VEHICLE_RMOTOR_POS_PIN,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    io_conf.pin_bit_mask=1ULL<<VEHICLE_RMOTOR_NEG_PIN;
    gpio_config(&io_conf);

    io_conf.pin_bit_mask=1ULL<<VEHICLE_LMOTOR_POS_PIN;
    gpio_config(&io_conf);
    io_conf.pin_bit_mask=1ULL<<VEHICLE_LMOTOR_NEG_PIN;
    gpio_config(&io_conf);
}

void Vehicle_Rmotor_change_speed(double duty)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, VEHICLE_RMOTOR_PWM_CHANNEL,
                  helper_duty2real(duty));
}

void Vehicle_Lmotor_change_speed(double duty)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, VEHICLE_LMOTOR_PWM_CHANNEL,
                  helper_duty2real(duty));
}

void Vehicle_Rmotor_stop()
{
    gpio_set_level(VEHICLE_RMOTOR_POS_PIN,0);
    gpio_set_level(VEHICLE_RMOTOR_NEG_PIN,0);
}

void Vehicle_Lmotor_stop()
{
    gpio_set_level(VEHICLE_LMOTOR_POS_PIN,0);
    gpio_set_level(VEHICLE_LMOTOR_NEG_PIN,0);
}

void Vehicle_Rmotor_forward()
{
    gpio_set_level(VEHICLE_RMOTOR_POS_PIN,1);
    gpio_set_level(VEHICLE_RMOTOR_NEG_PIN,0);
}

void Vehicle_Lmotor_forward()
{
    gpio_set_level(VEHICLE_LMOTOR_POS_PIN,1);
    gpio_set_level(VEHICLE_LMOTOR_NEG_PIN,0);
}

void Vehicle_Rmotor_backward()
{
    gpio_set_level(VEHICLE_RMOTOR_POS_PIN,0);
    gpio_set_level(VEHICLE_RMOTOR_NEG_PIN,1);
}

void Vehicle_Lmotor_backward()
{
    gpio_set_level(VEHICLE_LMOTOR_POS_PIN,0);
    gpio_set_level(VEHICLE_LMOTOR_NEG_PIN,1);
}

// void Vehicle_motor_control(char state){
    
// }