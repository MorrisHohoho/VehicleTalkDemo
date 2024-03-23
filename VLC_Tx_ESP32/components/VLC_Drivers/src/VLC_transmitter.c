#include "VLC_transmitter.h"
#include "VLC_parameters.h"
#include "VLC_encoder.h"
#include "VLC_timer.h"
#include "rom/ets_sys.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include <string.h>

static int VLC_transmitter_tx1_pin = GPIO_NUM_19;
static int VLC_transmitter_tx2_pin = GPIO_NUM_0;

static uint8_t VLC_data_idle[VLC_PAYLOAD_LENGTH]; // 0XAA, for DC-balance

void VLC_transmitter_init()
{
    // 1. configure the UART
    const uart_port_t uart_num1 = UART_NUM_1;
    const uart_port_t uart_num2 = UART_NUM_2;

    uart_config_t uart_config = {
        .baud_rate = VLC_BAUD_RATE,
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

    ESP_ERROR_CHECK(uart_param_config(uart_num1, &uart_config));
    ESP_ERROR_CHECK(uart_param_config(uart_num2, &uart_config));

    // 2. set the pin for UART
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1,
                                 VLC_transmitter_tx1_pin,
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE));

    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2,
                                 VLC_transmitter_tx2_pin,
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE));

    // 3. setup the UART buffered IO with event queue
    const int uart_buffer_size = 1024;
    // 4. Install UART driver using an event queue here
    if (!uart_is_driver_installed(UART_NUM_1))
    {
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1,
                                            uart_buffer_size,
                                            uart_buffer_size,
                                            10,
                                            NULL,
                                            intr_alloc_flags));
    }
    if (!uart_is_driver_installed(UART_NUM_2))
    {
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2,
                                            uart_buffer_size,
                                            uart_buffer_size,
                                            10,
                                            NULL,
                                            intr_alloc_flags));
    }

    /* Configure the idle signal buffer */
    memset(VLC_data_idle,0xAA,VLC_PAYLOAD_LENGTH);
}

void VLC_transmitter_DoSend(const char *data, TransmitterFlag flag, int arr_len)
{
    uint16_t frame_counts = arr_len / VLC_PAYLOAD_LENGTH;
    if (arr_len % VLC_PAYLOAD_LENGTH != 0)
    {
        frame_counts++;
    }

    uint16_t phy_frame_length = VLC_FRAME_LENGTH * 2 + 1;
    uint8_t tx_buf[phy_frame_length];

    uint64_t VLC_bit_duration_temp = 1000000/VLC_BAUD_RATE;
    uint64_t VLC_header_duration = VLC_bit_duration_temp*8;
    for (uint16_t i = 0; i < frame_counts; i++)
    {
        // printf("S:%ld\n",get_VLC_general_timer_count());
        VLC_encoder_DoEncode(data + i * VLC_PAYLOAD_LENGTH,tx_buf);
        // printf("E:%ld\n",get_VLC_general_timer_count());
        switch (flag)
        {
        case VLC_TX1:
        {
            // Transmit Header
            ets_delay_us(VLC_header_duration);
            // Transmit Data.
            uart_write_bytes(UART_NUM_1, tx_buf+1, phy_frame_length);
            uart_wait_tx_done(UART_NUM_1,portMAX_DELAY);
            break;
        }
        case VLC_TX2:
        {
            ets_delay_us(VLC_header_duration);
            uart_write_bytes(UART_NUM_2, tx_buf+1, phy_frame_length);
            uart_wait_tx_done(UART_NUM_2,portMAX_DELAY);
            break;
        }

        case VLC_TX_BOTH:
        {
            uart_write_bytes(UART_NUM_1, tx_buf+1, phy_frame_length);
            uart_write_bytes(UART_NUM_2, tx_buf+1, phy_frame_length);
            break;
        }
        }
    }
}

void VLC_transmitter_DoIdle(TransmitterFlag flag)
{
    
    switch (flag)
    {
    case VLC_TX1:
    {
        uart_write_bytes(UART_NUM_1, VLC_data_idle, VLC_PAYLOAD_LENGTH);
        break;
    }
    case VLC_TX2:
    {
        uart_write_bytes(UART_NUM_2, VLC_data_idle, VLC_PAYLOAD_LENGTH);
        break;
    }
    case VLC_TX_BOTH:
    {
        uart_write_bytes(UART_NUM_1, VLC_data_idle, VLC_PAYLOAD_LENGTH);
        uart_write_bytes(UART_NUM_2, VLC_data_idle, VLC_PAYLOAD_LENGTH);
        break;
    }
    }
}
