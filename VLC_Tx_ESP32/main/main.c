#include "TX_Timer.h"
#include "driver/gpio.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"

#include <string.h>
#include <unistd.h>

#define TX3_GPIO_NUM GPIO_NUM_0
#define TX1_GPIO_NUM GPIO_NUM_19
#define VLC_BAUD_RATE 256000

#define DATA_LENGTH 59

void app_main(void)
{
    // const char *mes = "IloveSCUANDIloveSCUMakerANDIloveVLCANDIloveVehicleTalkANDIloveRaptorCodeBUTIhateSpinalCodeBeacuseIt'suselessANDIloveMath:0123456789Somethinglikethat";
    // VLC_transmitter_init(TX3_GPIO_NUM, VLC_BAUD_RATE, DATA_LENGTH);
    // while (1)
    // {
    //     VLC_transmitter_send(mes);
    // }


    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = 1<<TX3_GPIO_NUM;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    TX3_gpitimer_init();

    printf("Test Timer\n");
    while(1){
        gpio_set_level(TX3_GPIO_NUM,1);
        TX3_udelay(100);
        gpio_set_level(TX3_GPIO_NUM,0);
        TX3_udelay(100);
    }
}
