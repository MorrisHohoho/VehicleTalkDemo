#include "transmitter.h"
#include "Tx_Timer.h"

#include "driver/uart.h"

static int VLC_tx_pin = 0;
static int VLC_baud_rate = 0;
static int VLC_data_packet_length = 0;
static int VLC_frame_length = 0;

#define TEST_UART_RX_PIN GPIO_NUM_14

void VLC_transmitter_init(const int tx_pin, const int baud_rate, const int data_length)
{
    VLC_tx_pin = tx_pin;
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
    int frame_in_bytes = VLC_frame_length * 2 + 2; //payload + header + protection byte
    const int uart_buffer_size = frame_in_bytes * 10;

    // 4. Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2,
                                        uart_buffer_size,
                                        uart_buffer_size,
                                        10,
                                        NULL,
                                        intr_alloc_flags));

    // 5. init the timer
    TX3_gpitimer_init();
}

void VLC_transmitter_send(const char *data)
{
    int frame_counts = strlen(data) / VLC_data_packet_length;
    if (strlen(data) % VLC_data_packet_length != 0)
    {
        frame_counts++;
    }

    uint16_t duration = 1000000 / VLC_baud_rate; //us
    duration *= 8;

    uint8_t tx_buf[VLC_frame_length * 2 + 2]; //payload after manchester encoding + 1 byte header + 1 byte tailer
    for (int i = 0; i < frame_counts; i++)
    {
        encode(data + i * VLC_data_packet_length, i, tx_buf);

        // for(int i=0;i<VLC_frame_length*2+2;i++){
        //     printf("%x ",tx_buf[i]);
        // }
        // printf("\n");

        //The tailer has to stop the UART transmitting
        uart_write_bytes(UART_NUM_2, tx_buf, VLC_frame_length * 2 + 2);

        // Transmit the tailer: 0b11111111
        TX3_udelay(duration);

    }
}