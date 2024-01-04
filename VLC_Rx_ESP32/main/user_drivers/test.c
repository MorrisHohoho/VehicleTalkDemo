#include "./include/test.h"

void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d\n", buffer[i]);
    }
    printf("-1\n");
}

void test_get_orignal_frame(uint8_t* buffer)
{
    // // 1. demodualte
    // uint8_t payload[PAYLOAD_LENGTH_BITS]={0};
    // demodulator_demodulate_OOK(buffer,payload,500);

    // // 2. decode Manchester Encoding
    // uint8_t payload_no_manchester[PAYLOAD_LENGTH_BITS/2]={0};
    // decode_manchester(payload,payload_no_manchester,PAYLOAD_LENGTH_BITS/2);

    // //3. covert bits to bytes to get frame
    // uint8_t payload_in_bytes[PAYLOAD_LENGTH_BYTES]={0};
    // decode_ascii(payload_no_manchester,payload_in_bytes,PAYLOAD_LENGTH_BYTES);

    // //4. print the frame
    // for(int i=0;i<PAYLOAD_LENGTH_BYTES;i++)
    // {
    //     printf("%d,",payload_in_bytes[i]);
    // }
    // printf("\n");

}
