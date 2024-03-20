#include "transmitter.h"
#include "encode.h"
#include "tim.h"
#include "usart.h"


#include <stdint.h>
#include <string.h>


void transmitter_send(const char* data)
{
	int frame_counts = strlen(data)/PAYLOAD_LEN;
	if(strlen(data)%PAYLOAD_LEN != 0)
	{
		frame_counts++;
	}

	uint16_t duration = 1000000/VLC_BUAD_RATE; //us
	duration*=8;
	
	uint8_t tx_buf[FRAME_LEN*2+2]; //payload after manchester encoding + 1 byte header + 1 byte tailer
	for(int i=0;i<frame_counts;i++)
	{
		encode(data+i*PAYLOAD_LEN,i,tx_buf);

		//The tailer has to stop the UART transmitting
		HAL_UART_Transmit(&huart1,tx_buf,FRAME_LEN*2+1,HAL_MAX_DELAY); 	// Header + payload

		// // Transmit the tailer: 0b11111111
		user_delay_us_tim(duration);
	}
}