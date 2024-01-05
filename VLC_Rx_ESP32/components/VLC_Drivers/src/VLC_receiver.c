#include "VLC_receiver.h"
#include "VLC_timer.h"
#include "VLC_parameters.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "rom/ets_sys.h"

static int VLC_receiver_rx1_pin = GPIO_NUM_8;
static int VLC_receiver_rx2_pin = GPIO_NUM_4;
static RecvState VLC_receiver_rx1_state = VLC_IDLE;
static RecvState VLC_receiver_rx2_state = VLC_IDLE;

extern gptimer_handle_t VLC_timer_rx1_handler;
extern gptimer_handle_t VLC_timer_rx2_handler;

#define VLC_RX1_UART_PORT UART_NUM_0
#define VLC_RX2_UART_PORT UART_NUM_2

/*
 * The logic of detecting the header.
 */
static void VLC_receiver_detect_header(RecvState *state, int current_level, gptimer_handle_t timer_handler)
{
    switch (*state)
    {
    case VLC_IDLE:
    {
        if (current_level == 1)
        {
            gptimer_set_raw_count(timer_handler, 0);
            ESP_ERROR_CHECK(gptimer_start(timer_handler));
            *state = VLC_PRE;
        }
        break;
    }

    case VLC_PRE:
    {
        gptimer_stop(timer_handler);
        gptimer_set_raw_count(timer_handler, 0);
        *state = VLC_IDLE;
        break;
    }

    case VLC_RECEIVING:
    {
        break;
    }
    }
}

static void IRAM_ATTR gpio_isr_handler_tim(void *arg)
{
    int pin = (int)arg;
    if (pin == VLC_receiver_rx1_pin)
    {
        VLC_receiver_detect_header(&VLC_receiver_rx1_state,
                                   gpio_get_level(pin),
                                   VLC_timer_rx1_handler);
    }
    else if (pin == VLC_receiver_rx2_pin)
    {
        VLC_receiver_detect_header(&VLC_receiver_rx2_state,
                                   gpio_get_level(pin),
                                   VLC_timer_rx2_handler);
    }
}

void VLC_receiver_init()
{
    // 1. configure the UART
    const uart_port_t uart_num2 = UART_NUM_2;
    const uart_port_t uart_num0 = UART_NUM_0;

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

    ESP_ERROR_CHECK(uart_param_config(uart_num0, &uart_config));
    ESP_ERROR_CHECK(uart_param_config(uart_num2, &uart_config));

    // 2. set the pin for UART
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0,
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
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0,
                                        uart_buffer_size,
                                        0,
                                        10,
                                        NULL,
                                        intr_alloc_flags));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2,
                                        uart_buffer_size,
                                        0,
                                        10,
                                        NULL,
                                        intr_alloc_flags));

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

    /* Configure the Timer */
    uint64_t VLC_bit_duration_temp = 1000000 / VLC_BAUD_RATE;
    VLC_timer_rx1N2_init(VLC_bit_duration_temp);
}

/**
 * Rewrite the __weak function in VLC_timer.c
 */
void IRAM_ATTR VLC_timer_PeriodElapsedCallback(gptimer_handle_t timer, void *arg)
{
    if (timer == VLC_timer_rx1_handler)
    {
        gptimer_stop(VLC_timer_rx1_handler);
        gpio_intr_disable(VLC_receiver_rx1_pin);
        gptimer_set_raw_count(VLC_timer_rx1_handler, 0);
        // uart_flush(VLC_RX1_UART_PORT);
        VLC_receiver_rx1_state = VLC_RECEIVING;
        // ets_printf("timer1\n");
    }
    else if (timer == VLC_timer_rx2_handler)
    {
        gptimer_stop(VLC_timer_rx2_handler);
        gpio_intr_disable(VLC_receiver_rx2_pin);
        gptimer_set_raw_count(VLC_timer_rx2_handler, 0);
        uart_flush(VLC_RX2_UART_PORT);
        VLC_receiver_rx2_state = VLC_RECEIVING;
        ets_printf("timer2\n");
    }
}


DataState VLC_receiver_DoReceive(uint8_t* rx1_buffer,uint8_t*  rx2_buffer)
{
    DataState ret = VLC_DATA_NONE;
    if(VLC_receiver_rx1_state==VLC_RECEIVING)
    {
        // uart_flush(VLC_RX1_UART_PORT);
        uart_read_bytes(VLC_RX1_UART_PORT,rx1_buffer,VLC_FRAME_LENGTH*2,portMAX_DELAY);
        uart_flush(VLC_RX1_UART_PORT);
        VLC_receiver_rx1_state = VLC_IDLE;
        ret = VLC_DATA_RX1;
        gpio_intr_enable(VLC_receiver_rx1_pin);
    }
    if(VLC_receiver_rx2_state==VLC_RECEIVING)
    {

        uart_read_bytes(VLC_RX2_UART_PORT,rx2_buffer,VLC_FRAME_LENGTH*2,portMAX_DELAY);
        VLC_receiver_rx2_state = VLC_IDLE;
        ret = ret==VLC_DATA_NONE? VLC_DATA_RX2: VLC_DATA_BOTH;
        gpio_intr_enable(VLC_receiver_rx2_pin);
    }

    return ret;
}