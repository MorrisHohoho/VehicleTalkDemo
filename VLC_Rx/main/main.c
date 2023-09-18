#include "demodulator.h"
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
#define VLC_BAUD_RATE 115200

#define PAYLOAD_LEN_BYTES 60

#define SYMBOLS_BUFFER_SIZE 130
#define MESSAGE_BUFFER_SIZE 6000

MessageBufferHandle_t MessageBuffer = NULL;

void vtask_read(void *ptParam)
{
    demodulator_config(TX_GPIO_NUM,PD_GPIO_NUM,VLC_BAUD_RATE);
    uint8_t send_buffer[SYMBOLS_BUFFER_SIZE];
    int send_bytes=0;
    while(1)
    {
        memset(send_buffer,0,SYMBOLS_BUFFER_SIZE*sizeof(uint8_t));
        demodulator_receive(send_buffer);
        send_bytes= xMessageBufferSend(MessageBuffer, 
                            (void*)send_buffer, 
                            SYMBOLS_BUFFER_SIZE, 
                            0);

        if(send_bytes==0)
        {
            xMessageBufferReset(MessageBuffer);
            printf("-2\n");
        }
    }
}

void vtask_operate(void *ptParam)
{

    uint8_t symbols_buffer[SYMBOLS_BUFFER_SIZE];
    uint8_t temp_buffer[SYMBOLS_BUFFER_SIZE];
    uint8_t tx_output[PAYLOAD_LEN_BYTES];
    int recv_bytes=0;
    while(1)
    {


        memset(symbols_buffer,0,SYMBOLS_BUFFER_SIZE*sizeof(uint8_t));
        memset(tx_output,0,33*sizeof(uint8_t));
        recv_bytes=xMessageBufferReceive(MessageBuffer, 
                                (void*)symbols_buffer, 
                                sizeof(symbols_buffer), 
                                portMAX_DELAY);

        if(recv_bytes!=0)
        {
            if(symbols_buffer[0]!=0x00)
            {
                memcpy(temp_buffer,symbols_buffer,SYMBOLS_BUFFER_SIZE*sizeof(uint8_t));
            }
            else
            {
                memcpy(temp_buffer,symbols_buffer+1,(SYMBOLS_BUFFER_SIZE-1)*sizeof(uint8_t));
            }
        // for(int i=0;i<118;i++)
        // {
        //     printf("%x ",temp_buffer[i]);
        // }
        // printf("\n");

        decode_manchester(temp_buffer,tx_output,PAYLOAD_LEN_BYTES*2);
        printf("%x:",tx_output[0]);
        for(int i=1;i<PAYLOAD_LEN_BYTES;i++)
        {
            printf("%c",tx_output[i]);
        }
        printf("\n");
        }
    }
}



void app_main(void)
{

    // uint8_t test_buffer[TEST_BUFFER_SIZE];
    // demodulator_config(27,35,115200);
    // while(1)
    // {
    //     test_transmit();
    //     test_receive();
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }

    MessageBuffer= xMessageBufferCreate(MESSAGE_BUFFER_SIZE*10);

    if(MessageBuffer !=NULL)
    {
    xTaskCreatePinnedToCore(vtask_read, "vtask_read", 8192, NULL, 1, NULL, 0);  // Read the Rx pin
    xTaskCreatePinnedToCore(vtask_operate, "vtask_operate", 8192, NULL, 1, NULL, 1);    // Decode the symbols and operate
    }
    else
    {
        printf("MessageBuffer create failed\n");
    }
}
