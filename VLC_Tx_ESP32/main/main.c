#include "VLC_transmitter.h"
#include "VLC_receiver.h"
#include "VLC_timer.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"
#include "rom/ets_sys.h"

#include <string.h>
#include <unistd.h>

bool isSend = false;

void IRAM_ATTR VLC_timer_generalCallback(gptimer_handle_t timer,void *arg)
{
    ets_printf("Send data\n");
    isSend=true;
}

void app_main(void)
{
    // const char *mes = "IloveSCUANDIloveSCUMakerANDIloveVLCANDIloveVehicleTalkANDIloveRaptorCodeBUTIhateSpinalCodeBeacuseIt'suselessANDIloveMath:0123456789Somethinglikethat";
    VLC_transmitter_init();
    VLC_receiver_init();
    printf("TX start\n");
    const char *mes = "ABCD1234";
    TransmitterFlag tx_state = VLC_TX2;
    VLC_timer_general_init(1000000);
    while(1)
    {
        VLC_transmitter_DoIdle(tx_state);
        if(isSend)
        {
            VLC_transmitter_DoSend(mes,VLC_TX2);
            isSend = false;
        }
    }

}
