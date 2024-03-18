#include "VLC_transmitter.h"
#include "VLC_timer.h"
#include "VLC_parameters.h"
#include "Vehicle_servo.h"
#include "Vehicle_motor.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"
#include "rom/ets_sys.h"

#include <string.h>
#include <unistd.h>

/* Testing Messages*/
const char* testing_8bytes_mes = "SCU:No1!";
const char* testing_16bytes_mes = "SCU?No1!no1?vlc!";
/* Testing Messages End*/

bool isSend = false;

void IRAM_ATTR VLC_timer_generalCallback(gptimer_handle_t timer, void *arg)
{
    isSend = true;
}

void app_main(void)
{
    // const char *mes = "IloveSCUANDIloveSCUMakerANDIloveVLCANDIloveVehicleTalkANDIloveRaptorCodeBUTIhateSpinalCodeBeacuseIt'suselessANDIloveMath:0123456789Somethinglikethat";
    VLC_transmitter_init();
    Vehicle_motor_init();
    Vehicle_Lmotor_stop();
    Vehicle_Rmotor_stop();
    Vehicle_servo_init();

    printf("TX start\n");
    TransmitterFlag tx_pin = VLC_TX2;
    VLC_timer_general_init(1500000);
    int test_temp_state = 0;

    while (1)
    {
        VLC_transmitter_DoSend(testing_16bytes_mes,tx_pin);
    }
}
