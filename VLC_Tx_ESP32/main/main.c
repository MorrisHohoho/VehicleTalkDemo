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
const char* testing_32bytes_mes = "SCU?No1!no1?vlc!SCU?No1!no1?vlc!";
const char* testing_64bytes_mes = "SCU?No1!no1?vlc!SCU?No1!no1?vlc!TCU?No1!no1?vlc!SCU?No1!no1?vlc!";
const char testing_temp_mes[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
const char testing_RS_160_32_mes[]={0, 83, 67, 85, 78, 79, 49, 107, 111, 78, 97, 118, 83, 52, 84, 115, 47, 84, 115, 68, 114, 82, 32, 65, 51, 60, 111, 83, 118, 111, 71, 116, 1, 54, 94, 123, 75, 109, 87, 75, 83, 48, 76, 100, 37, 59, 73, 55, 61, 97, 53, 109, 42, 87, 65, 80, 107, 114, 40, 76, 60, 33, 35, 43, 2, 81, 124, 32, 121, 37, 87, 99, 52, 71, 112, 94, 73, 100, 106, 95, 107, 121, 53, 76, 91, 54, 52, 106, 43, 81, 57, 93, 65, 112, 115, 74, 3, 114, 70, 35, 74, 112, 45, 87, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 220, 127, 182, 92, 130, 86, 204, 1, 26, 228, 108, 237, 11, 190, 206, 198, 188, 40, 113, 236, 7, 41, 55, 235, 198, 39, 19, 5, 162, 186, 196, 254, 201};
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
    VLC_timer_general_init(1500000,true);
    int test_temp_state = 0;

    while (1)
    {
        VLC_transmitter_DoSend(testing_RS_160_32_mes,tx_pin,sizeof(testing_RS_160_32_mes));
    }
}
