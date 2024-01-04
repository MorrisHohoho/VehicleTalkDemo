#include "transmitter.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"

#include <string.h>
#include <unistd.h>

#define VLC_TX1_GPIO GPIO_NUM_39
#define VLC_BAUD_RATE 115200
#define DATA_PACKET_LENGTH 4

void app_main(void)
{
    // const char *mes = "IloveSCUANDIloveSCUMakerANDIloveVLCANDIloveVehicleTalkANDIloveRaptorCodeBUTIhateSpinalCodeBeacuseIt'suselessANDIloveMath:0123456789Somethinglikethat";
    const char *mes = "ABCD";
    VLC_transmitter_init(VLC_TX1_GPIO, VLC_BAUD_RATE, DATA_PACKET_LENGTH);
    while (1)
    {
        VLC_transmitter_send(mes);
    }
}
