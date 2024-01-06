#include "VLC_transmitter.h"
#include "VLC_receiver.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"

#include <string.h>
#include <unistd.h>


void app_main(void)
{
    // const char *mes = "IloveSCUANDIloveSCUMakerANDIloveVLCANDIloveVehicleTalkANDIloveRaptorCodeBUTIhateSpinalCodeBeacuseIt'suselessANDIloveMath:0123456789Somethinglikethat";
    const char *mes = "ABCDEFG";
    // uint8_t tx_buf[100];
    VLC_transmitter_init();
    VLC_receiver_init();
    TransmitterFlag tx_state = VLC_TX2;
    printf("TX start\n");
    while(1)
    {
        VLC_transmitter_DoSend(mes,tx_state);
    }

}
