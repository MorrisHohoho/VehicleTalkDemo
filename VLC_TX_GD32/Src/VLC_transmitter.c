//
// Created by Administrator on 11/30/2023.
//

#include "VLC_transmitter.h"
#include "VLC_encoder.h"
#include "VLC_timer.h"
#include "main.h"

#include "usart.h"
#include <string.h>

void VLC_transmitter_send(const char* data)
{
    int frame_counts = strlen(data)/PAYLOAD_LENGTH;
    if(strlen(data)%PAYLOAD_LENGTH != 0)
    {
        frame_counts++;
    }

    uint16_t duration = 1000000/VLC_BUAD_RATE; //us
    duration*=8;

    uint8_t tx_buf[FRAME_LENGTH*2+2]; //payload after manchester encoding + 1 byte header + 1 byte tailer
    for(int i=0;i<frame_counts;i++)
    {
        VLC_encode(data+i*PAYLOAD_LENGTH,i,tx_buf);

        //The tailer has to stop the UART transmitting
        HAL_UART_Transmit(&huart4,tx_buf,FRAME_LENGTH*2+2,HAL_MAX_DELAY); 	// Header + payload

        //Transmit the tailer: 0b11111111
        VLC_udelay(duration);
    }
}