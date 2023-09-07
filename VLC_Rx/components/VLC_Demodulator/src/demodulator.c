#include "demodulator.h"

#include "driver/uart.h"

static int VLC_rx_pin = 0;
void demodulator_config(const int tx_pin, const int rx_pin,const int baud_rate)
{
    VLC_rx_pin = rx_pin;
    // 1. configure the UART
    const uart_port_t uart_num = UART_NUM_2;

    uart_config_t uart_config={
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits =UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

    #if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
    #endif

    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    // 2. set the pin for UART
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2,tx_pin,rx_pin,UART_PIN_NO_CHANGE,UART_PIN_NO_CHANGE));

    // 3. setup the UART buffered IO with event queue
    const int uart_buffer_size = (FRAME_IN_BYTES+4)*10;

    // 4. Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, 
                                        uart_buffer_size, 
                                        uart_buffer_size, 
                                        10, 
                                        NULL,
                                        intr_alloc_flags));

    /*Configure the Clock*/
    uint64_t duration = 1000000/baud_rate;
    header_clock_init(duration,rx_pin);
    
    /*Configure the ISR of the GPIO*/
    gpio_config_t io_conf={
        .pin_bit_mask=1ULL<<rx_pin,
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_NEGEDGE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&io_conf);
    gpio_install_isr_service(intr_alloc_flags);
    gpio_isr_handler_add(rx_pin,gpio_isr_handler_tim,NULL);


}

void demodulator_receive(uint8_t *buffer)
{
    set_header_detect_state(false);
    gpio_set_intr_type(VLC_rx_pin,GPIO_INTR_NEGEDGE);
    gpio_intr_enable(VLC_rx_pin);
    // uart_write_bytes(UART_NUM_2,"0",1);
    while(1)
    {
        if(get_header_detect_state())
        {
            uart_flush(UART_NUM_2);
            uart_read_bytes(UART_NUM_2, buffer, 119, portMAX_DELAY);
            // uart_write_bytes(UART_NUM_2,"2",1);
            break;
        }
    }
}
static void IRAM_ATTR gpio_isr_handler_tim(void *ard)
{
    switch(header_clock_get_state())
    {
        case false:
        {
            gpio_intr_disable(VLC_rx_pin);
            header_clock_start();
            gpio_set_intr_type(VLC_rx_pin,GPIO_INTR_POSEDGE);
            gpio_intr_enable(VLC_rx_pin);
            break;
        }
        case true:
        {
            gpio_intr_disable(VLC_rx_pin);
            header_clock_stop();
            gpio_set_intr_type(VLC_rx_pin,GPIO_INTR_NEGEDGE);
            gpio_intr_enable(VLC_rx_pin);
            break;
        }
    }
}