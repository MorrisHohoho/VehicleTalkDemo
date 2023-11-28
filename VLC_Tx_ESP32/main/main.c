#include "transmitter.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"

#include <string.h>
#include <unistd.h>

#define TX3_GPIO_NUM GPIO_NUM_0
#define TX1_GPIO_NUM GPIO_NUM_19
#define VLC_BAUD_RATE 115200
#define DATA_PACKET_LENGTH 59

void app_main(void)
{
    // const char *mes = "IloveSCUANDIloveSCUMakerANDIloveVLCANDIloveVehicleTalkANDIloveRaptorCodeBUTIhateSpinalCodeBeacuseIt'suselessANDIloveMath:0123456789Somethinglikethat";
    const char *mes = "ABCD";
    VLC_transmitter_init(TX3_GPIO_NUM, VLC_BAUD_RATE, DATA_PACKET_LENGTH);
    while (1)
    {
        VLC_transmitter_send(mes);
    }
}
