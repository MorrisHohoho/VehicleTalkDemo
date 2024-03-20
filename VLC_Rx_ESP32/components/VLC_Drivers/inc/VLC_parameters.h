#ifndef __VLC_PARAMETERS_H__
#define __VLC_PARAMETERS_H__


#define VLC_PAYLOAD_LENGTH 32 // The original data length, in bytes. i.e.: "abcd" length is 4.
#define VLC_FRAME_LENGTH  (VLC_PAYLOAD_LENGTH) // The frame data length (add 1 bytes PSN), in bytes. i.e. : 1+"abdc" length is 5.

#define VLC_FRAME_HEADER 0x00
#define VLC_FRAME_TAIL 0X55

#define VLC_BAUD_RATE 40000

#endif