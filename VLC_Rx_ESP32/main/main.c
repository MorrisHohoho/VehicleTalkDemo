#include "VLC_receiver.h"
#include "VLC_decoder.h"
#include "VLC_parameters.h"

#include "Vehicle_motor.h"
#include "Vehicle_servo.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"

#include <string.h>
#include <unistd.h>

#define SYMBOLS_BUFFER_SIZE (VLC_FRAME_LENGTH * 2 + 4)
#define MESSAGE_BUFFER_SIZE 6000
#define VERBOSE_OUTPUT 0

MessageBufferHandle_t MessageBuffer = NULL;

void vtask_read(void *ptParam)
{
    printf("Rx start\n");
    printf("BAUD_RATE:%d\n",VLC_BAUD_RATE);
    printf("PAYLOAD_Length:%d\n",VLC_PAYLOAD_LENGTH);
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
            #if VERBOSE_OUTPUT
            printf("Ori:");
            for(int i=0;i<VLC_FRAME_LENGTH * 2+2;i++)
            {
                printf("%x ",rx1_buffer[i]);
            }
            printf("\n");
            #endif
                xMessageBufferSend(MessageBuffer,
                                   (void *)&rx1_buffer, // discard the header
                                   VLC_FRAME_LENGTH * 2,
                                   0);
            break;
        }
        case VLC_DATA_RX2:
        {
                xMessageBufferSend(MessageBuffer,
                                   (void *)&rx2_buffer, // discard the header
                                   VLC_FRAME_LENGTH * 2,
                                   0);
            break;
        }
        default:
            break;
        }
    }
}

void vtask_operate(void *ptParam)
{
    Vehicle_motor_init();
    Vehicle_Rmotor_stop();
    Vehicle_Lmotor_stop();
    Vehicle_servo_init();
    Vehicle_servo_change_angle(90);

    uint8_t temp_recv_buffer[SYMBOLS_BUFFER_SIZE];
    uint8_t tx_output[VLC_FRAME_LENGTH];
    int recv_bytes = 0;
    while (1)
    {

        memset(temp_recv_buffer, 0, SYMBOLS_BUFFER_SIZE * sizeof(uint8_t));
        recv_bytes = xMessageBufferReceive(MessageBuffer,
                                           (void *)temp_recv_buffer,
                                           sizeof(temp_recv_buffer),
                                           portMAX_DELAY);

        if (recv_bytes != 0)
        {
            VLC_decoder_Dodecode(temp_recv_buffer, tx_output);
            #if !VERBOSE_OUTPUT
            for (int i = 0; i < VLC_FRAME_LENGTH; i++)
            {
                printf("%d,", tx_output[i]);
            }
            printf("\n");
            #endif

            // Vehicle_motor_control(tx_output[1]);
        }
    }
}

void app_main(void)
{
    MessageBuffer = xMessageBufferCreate(MESSAGE_BUFFER_SIZE * 10);

    if (MessageBuffer != NULL)
    {
        xTaskCreatePinnedToCore(vtask_read, "vtask_read", 8192, NULL, 1, NULL, 0);       // Read the Rx pin
        xTaskCreatePinnedToCore(vtask_operate, "vtask_operate", 8192, NULL, 1, NULL, 1); // Decode the symbols and operate
    }
    else
    {
        printf("MessageBuffer create failed\n");
    }
}
