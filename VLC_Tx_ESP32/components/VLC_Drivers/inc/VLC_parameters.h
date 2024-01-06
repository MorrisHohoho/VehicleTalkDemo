#ifndef VLC_PARAMETERS_H
#define VLC_PARAMETERS_H


#define VLC_PAYLOAD_LENGTH 8 //The original data length, in bytes. i.e.: "abcd" length is 4.
#define VLC_FRAME_LENGTH  (VLC_PAYLOAD_LENGTH+1) //The frame data length (add 1 bytes PSN), in bytes. i.e. : 1+"abdc" length is 5.

#define VLC_BAUD_RATE 115200


#endif