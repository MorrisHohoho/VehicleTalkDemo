#include "VLC_receiver.h"
#include "VLC_decoder.h"
#include "VLC_parameters.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"

#include <string.h>
#include <unistd.h>

#define SYMBOLS_BUFFER_SIZE 130
#define MESSAGE_BUFFER_SIZE 6000

MessageBufferHandle_t MessageBuffer = NULL;

void vtask_read(void *ptParam)
{
    VLC_receiver_init();
    uint8_t rx1_buffer[SYMBOLS_BUFFER_SIZE];
    uint8_t rx2_buffer[SYMBOLS_BUFFER_SIZE];
    DataState data_state = VLC_DATA_NONE;
    while (1)
    {
        data_state = VLC_receiver_DoReceive(rx1_buffer, rx2_buffer);
        switch (data_state)
        {
        case VLC_DATA_RX1:
        {
            for (int i = 0; i < VLC_FRAME_LENGTH * 2; i++)
            {
                printf("%x ", rx1_buffer[i]);
            }
            printf("\n");
            break;
        }
        case VLC_DATA_RX2:
        {
            for (int i = 0; i < VLC_FRAME_LENGTH * 2; i++)
            {
                printf("%x ", rx2_buffer[i]);
            }
            printf("\n");
            break;
        }
        default:
            break;
        }
    }

    // VLC_demodulator_config(VLC_RX1_GPIO_NUM,VLC_BAUD_RATE,VLC_FRAME_LENGTH);
    // demodulator_config(TX_GPIO_NUM,VLC_RX1_GPIO_NUM,VLC_BAUD_RATE);
    // uint8_t send_buffer[SYMBOLS_BUFFER_SIZE];
    // int send_bytes=0;
    // while(1)
    // {
    //     memset(send_buffer,0,SYMBOLS_BUFFER_SIZE*sizeof(uint8_t));
    //     demodulator_receive(send_buffer);
    //     send_bytes= xMessageBufferSend(MessageBuffer,
    //                         (void*)send_buffer,
    //                         SYMBOLS_BUFFER_SIZE,
    //                         0);

    //     if(send_bytes==0)
    //     {
    //         xMessageBufferReset(MessageBuffer);
    //         printf("-2\n");
    //     }
    // }
}

void vtask_operate(void *ptParam)
{

    // uint8_t symbols_buffer[SYMBOLS_BUFFER_SIZE];
    // uint8_t temp_buffer[SYMBOLS_BUFFER_SIZE];
    // uint8_t tx_output[VLC_FRAME_LENGTH];
    // int recv_bytes=0;
    // while(1)
    // {

    //     memset(symbols_buffer,0,SYMBOLS_BUFFER_SIZE*sizeof(uint8_t));
    //     memset(tx_output,0,33*sizeof(uint8_t));
    //     recv_bytes=xMessageBufferReceive(MessageBuffer,
    //                             (void*)symbols_buffer,
    //                             sizeof(symbols_buffer),
    //                             portMAX_DELAY);

    //     if(recv_bytes!=0)
    //     {
    //         if(symbols_buffer[0]!=0x00)
    //         {
    //             memcpy(temp_buffer,symbols_buffer,SYMBOLS_BUFFER_SIZE*sizeof(uint8_t));
    //         }
    //         else
    //         {
    //             memcpy(temp_buffer,symbols_buffer+1,(SYMBOLS_BUFFER_SIZE-1)*sizeof(uint8_t));
    //         }
    //     // for(int i=0;i<118;i++)
    //     // {
    //     //     printf("%x ",temp_buffer[i]);
    //     // }
    //     // printf("\n");

    //     decode_manchester(temp_buffer,tx_output,VLC_FRAME_LENGTH*2);
    //     printf("%x:",tx_output[0]);
    //     for(int i=1;i<VLC_FRAME_LENGTH;i++)
    //     {
    //         printf("%c",tx_output[i]);
    //     }
    //     printf("\n");
    //     }
    // }
}

void app_main(void)
{
    MessageBuffer = xMessageBufferCreate(MESSAGE_BUFFER_SIZE * 10);

    if (MessageBuffer != NULL)
    {
        xTaskCreatePinnedToCore(vtask_read, "vtask_read", 8192, NULL, 1, NULL, 0); // Read the Rx pin
        // xTaskCreatePinnedToCore(vtask_operate, "vtask_operate", 8192, NULL, 1, NULL, 1);    // Decode the symbols and operate
    }
    else
    {
        printf("MessageBuffer create failed\n");
    }
}
