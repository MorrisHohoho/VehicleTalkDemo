#ifndef __VLC_PARAMETERS_H__
#define __VLC_PARAMETERS_H__


#define VLC_PAYLOAD_LENGTH 8 // The original data length, in bytes. i.e.: "abcd" length is 4.
#define VLC_FRAME_LENGTH  (VLC_PAYLOAD_LENGTH+1) // The frame data length (add 1 bytes PSN), in bytes. i.e. : 1+"abdc" length is 5.

#define VLC_FRAME_HEADER 0x1F
#define VLC_FRAME_TAIL 0XAA

#define VLC_BAUD_RATE 180000


#endif