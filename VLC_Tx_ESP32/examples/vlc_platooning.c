/**
 * The simple platooning demo based on VLC.
 * @author MorrisHohoho
*/

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
    TransmitterFlag tx_state = VLC_TX2;
    VLC_timer_general_init(1500000);
    int test_temp_state = 0;

    while (1)
    {
        VLC_transmitter_DoIdle(tx_state);
        if (isSend)
        {
            switch (test_temp_state)
            {
            case 0:
            {
                VLC_transmitter_DoSend("Xtop1111", tx_state);
                Vehicle_motor_control('X');
                test_temp_state++;
                break;
            }
            case 1:
            {
                VLC_transmitter_DoSend("WXXXSCU!", tx_state);
                Vehicle_motor_control('W');
                test_temp_state++;
                break;
            }
            case 2:
            {
                VLC_transmitter_DoSend("Aeftturn", tx_state);
                Vehicle_motor_control('A');
                test_temp_state++;
                break;
            }
            case 3:
            {
                VLC_transmitter_DoSend("WYYYWIN!", tx_state);
                Vehicle_motor_control('W');
                test_temp_state++;
                break;
            }
            case 4:
            {
                VLC_transmitter_DoSend("DightHAX", tx_state);
                Vehicle_motor_control('D');
                test_temp_state++;
                break;
            }
            case 5:
            {
                VLC_transmitter_DoSend("WYYYWIN!", tx_state);
                Vehicle_motor_control('W');
                test_temp_state++;
                break;
            }
            case 6:
            {
                VLC_transmitter_DoSend("Xtop0011", tx_state);
                Vehicle_motor_control('X');
                test_temp_state++;
                break;
            }
            case 7:
            {
                VLC_transmitter_DoSend("Sback123", tx_state);
                Vehicle_motor_control('S');
                test_temp_state++;
                break;
            }
            case 8:
            {
                VLC_transmitter_DoSend("XOOO9988", tx_state);
                Vehicle_motor_control('X');
                test_temp_state = 0;
                break;
            }
            }
            isSend = false;
        }
    }
}
