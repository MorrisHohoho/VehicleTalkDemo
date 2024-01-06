#ifndef VLC_TRANSMITTER_H
#define VLC_TRANSMITTER_H

typedef enum{
    VLC_TX1,
    VLC_TX2,
    VLC_TX_BOTH,
}TransmitterFlag;

void VLC_transmitter_init();

void VLC_transmitter_DoSend(const char *data, TransmitterFlag flag);

void VLC_transmitter_DoIdle(TransmitterFlag flag);

#endif