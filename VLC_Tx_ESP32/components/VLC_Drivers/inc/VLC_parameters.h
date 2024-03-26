#ifndef __VLC_PARAMETERS_H__
#define __VLC_PARAMETERS_H__



#define VLC_PAYLOAD_LENGTH 32 // The original data length, in bytes. i.e.: "abcd" length is 4.
#define VLC_FRAME_LENGTH  (VLC_PAYLOAD_LENGTH) // Why we need frame number in the physical? We can just add it in the above layer.

#define VLC_FRAME_HEADER 0xFF
#define VLC_FRAME_TAIL 0X00

#define VLC_BAUD_RATE 120000        // Maxinum is 160000


#endif