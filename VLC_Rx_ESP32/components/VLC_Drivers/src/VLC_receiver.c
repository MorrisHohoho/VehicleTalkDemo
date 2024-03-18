#include "VLC_receiver.h"
#include "VLC_timer.h"
#include "VLC_parameters.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "rom/ets_sys.h"

#include <math.h>
#include <string.h>

static int VLC_receiver_rx1_pin = GPIO_NUM_8;
static int VLC_receiver_rx2_pin = GPIO_NUM_1;
static RecvState VLC_receiver_rx1_state = VLC_IDLE;
static RecvState VLC_receiver_rx2_state = VLC_IDLE;

extern gptimer_handle_t VLC_timer_rx1_handler;
extern gptimer_handle_t VLC_timer_rx2_handler;

#define VLC_RX1_UART_PORT UART_NUM_1
#define VLC_RX2_UART_PORT UART_NUM_2

#define VLC_RECEIVER_DEBUG 0

/*
 * Inner function
 * The logic of detecting the header.
 */
static void VLC_receiver_detect_header(RecvState *state, gptimer_handle_t timer_handler)
{
    switch (*state)
    {
    case VLC_IDLE:
    {
            gptimer_set_raw_count(timer_handler, 0);
            ESP_ERROR_CHECK(gptimer_start(timer_handler));
            *state = VLC_PRE;
        break;
    }

    case VLC_PRE:
    {
        // gptimer_stop(timer_handler);
        gptimer_set_raw_count(timer_handler, 0);
        // *state = VLC_IDLE;
        break;
    }

    case VLC_RECEIVING:
    {
        break;
    }
    }
}

/**
* Gpio interrupt handler, which is used to detect the header.
* Positive edge interrupt.
*/
static void IRAM_ATTR gpio_isr_handler_tim(void *arg)
{
    int pin = (int)arg;
    if (pin == VLC_receiver_rx1_pin)
    {
        VLC_receiver_detect_header(&VLC_receiver_rx1_state,
                                   VLC_timer_rx1_handler);
    }
    else if (pin == VLC_receiver_rx2_pin)
    {
        VLC_receiver_detect_header(&VLC_receiver_rx2_state,
                                   VLC_timer_rx2_handler);
    }
}

void VLC_receiver_init()
{
    // 1. configure the UART
    const uart_port_t uart_num2 = UART_NUM_2;
    const uart_port_t uart_num1 = UART_NUM_1;

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
                                 UART_PIN_NO_CHANGE,
                                 VLC_receiver_rx1_pin,
                                 UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE));

    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2,
                                 UART_PIN_NO_CHANGE,
                                 VLC_receiver_rx2_pin,
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
    
    /* Key UART buffer bug fixed */
    uart_set_rx_full_threshold(UART_NUM_1,VLC_FRAME_LENGTH*2+1);  // No need to store the tailer.
    uart_set_rx_full_threshold(UART_NUM_2,VLC_FRAME_LENGTH*2+1);

    /* Configure the Timer */
    // double XXtemp = 1000000/ (double) VLC_BAUD_RATE;
    // uint64_t VLC_bit_duration_temp = round(XXtemp);

    uint64_t VLC_bit_duration_temp = 1000000/VLC_BAUD_RATE;
    VLC_timer_rx1N2_init(VLC_bit_duration_temp);

    /*Configure the ISR of the GPIO*/
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << VLC_receiver_rx1_pin,
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_POSEDGE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&io_conf);
    gpio_install_isr_service(intr_alloc_flags);
    gpio_isr_handler_add(VLC_receiver_rx1_pin, gpio_isr_handler_tim,
                         (void *)VLC_receiver_rx1_pin);

    io_conf.pin_bit_mask = 1ULL << VLC_receiver_rx2_pin;
    gpio_config(&io_conf);
    gpio_isr_handler_add(VLC_receiver_rx2_pin, gpio_isr_handler_tim,
                         (void *)VLC_receiver_rx2_pin);

}

/**
 * Rewrite the __weak function in VLC_timer.c, also used to detect the header.
 */
void IRAM_ATTR VLC_timer_PeriodElapsedCallback(gptimer_handle_t timer, void *arg)
{
    if (timer == VLC_timer_rx1_handler)
    {
        gptimer_stop(VLC_timer_rx1_handler);
        gpio_intr_disable(VLC_receiver_rx1_pin);

        #if VLC_RECEIVER_DEBUG
        gpio_set_level(GPIO_NUM_18,1);
        #endif
        gptimer_set_raw_count(VLC_timer_rx1_handler, 0);
        uart_flush(VLC_RX1_UART_PORT);
        VLC_receiver_rx1_state = VLC_RECEIVING;
    }
    else if (timer == VLC_timer_rx2_handler)
    {
        gptimer_stop(VLC_timer_rx2_handler);
        gpio_intr_disable(VLC_receiver_rx2_pin);
        gptimer_set_raw_count(VLC_timer_rx2_handler, 0);
        uart_flush(VLC_RX2_UART_PORT);
        VLC_receiver_rx2_state = VLC_RECEIVING;
    }
}

DataState VLC_receiver_DoReceive(uint8_t *rx1_buffer, uint8_t *rx2_buffer)
{
    DataState ret = VLC_DATA_NONE;
    if (VLC_receiver_rx1_state == VLC_RECEIVING)
    {

        #if VLC_RECEIVER_DEBUG
        gpio_set_level(GPIO_NUM_18,0);
        #endif
        
        uart_read_bytes(VLC_RX1_UART_PORT, rx1_buffer, VLC_FRAME_LENGTH*2+1, portMAX_DELAY);

        VLC_receiver_rx1_state = VLC_IDLE;
        ret = VLC_DATA_RX1;
        gpio_intr_enable(VLC_receiver_rx1_pin);
    }
    if (VLC_receiver_rx2_state == VLC_RECEIVING)
    {
        // The lost byte will be filled in 0XAA.
        int length = uart_read_bytes(VLC_RX2_UART_PORT, rx2_buffer, VLC_FRAME_LENGTH * 2+1, portMAX_DELAY);
        
        VLC_receiver_rx2_state = VLC_IDLE;
        ret = ret == VLC_DATA_NONE ? VLC_DATA_RX2 : VLC_DATA_BOTH;
        gpio_intr_enable(VLC_receiver_rx2_pin);
    }

    return ret;
}
