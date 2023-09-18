#include "transmitter.h"
#include "./include/test.h"


#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"

#include <string.h>
#include <unistd.h>

#define TX_GPIO_NUM 27
#define PD_GPIO_NUM 35
#define VLC_BAUD_RATE 256000

#define DATA_LENGTH 59

void app_main(void)
{
const char* mes = "IloveSCUANDIloveSCUMakerANDIloveVLCANDIloveVehicleTalkANDIloveRaptorCodeBUTIhateSpinalCodeBeacuseIt'suselessANDIloveMath:0123456789Somethinglikethat";
    VLC_transmitter_init(TX_GPIO_NUM,VLC_BAUD_RATE,DATA_LENGTH);
    while(1)
    {
        VLC_transmitter_send(mes);
    }
}
