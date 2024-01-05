#include "transmitter.h"
#include "Tx_Timer.h"

#include "driver/uart.h"

static uint16_t VLC_tx1_pin = 0;
static int VLC_baud_rate = 0;

// The original data length, i.e., "ABCD" = 4.
static uint16_t VLC_data_packet_length = 0;   

/* The transmitted data length, which is one more longer than data_length due to the PSN.
*  i.e.,  "0ABCD" = 5. "0" represented that the packet sequence number is 0.
*/
static uint16_t VLC_frame_length = 0;        

#define TEST_UART_RX_PIN GPIO_NUM_14

void VLC_transmitter_init(const uint16_t tx_pin, const int baud_rate, const uint16_t data_length)
{
    VLC_tx1_pin = tx_pin;
    VLC_baud_rate = baud_rate;
    VLC_data_packet_length = data_length;
    VLC_frame_length = VLC_data_packet_length + 1; //data + 1 byte PSN
    // 1. configure the UART
    const uart_port_t uart_num = UART_NUM_2;

    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    // 2. set the pin for UART
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, tx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // 3. setup the UART buffered IO with event queue
    uint16_t frame_in_bytes = VLC_frame_length * 2 + 2; //payload + header + tailer
    const uint16_t uart_buffer_size = 1024;

    // 4. Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2,
                                        uart_buffer_size,
                                        uart_buffer_size,
                                        10,
                                        NULL,
                                        intr_alloc_flags));

    // 5. init the timer
    TX3_gpitimer_init();

    // 6. init the encoder;
    vlc_encoder_init(data_length);
}

void VLC_transmitter_send(const char *data)
{
    uint16_t frame_counts = strlen(data) / VLC_data_packet_length;
    if (strlen(data) % VLC_data_packet_length != 0)
    {
        frame_counts++;
    }

    uint64_t VLC_duration = 1000000 / VLC_baud_rate; //us
    VLC_duration *= 10;

    uint8_t tx_buf[VLC_frame_length * 2 + 2]; //payload after manchester encoding + 1 byte header + 1 byte tailer
    for (uint16_t i = 0; i < frame_counts; i++)
    {
        vlc_encode(data + i * VLC_data_packet_length, i, tx_buf);

        // for(int i=0;i<VLC_frame_length*2+2;i++){
        //     printf("%x ",tx_buf[i]);
        // }
        // printf("\n");

        // Transmit the data and the tailor.
        uart_write_bytes(UART_NUM_2, tx_buf, VLC_frame_length * 2 + 2);

    }
}