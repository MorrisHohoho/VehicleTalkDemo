#include "clock.h"


static int clock_rx_pin = 0;
static bool header_start_receving()
{
    gpio_intr_disable(clock_rx_pin);
    header_clock_stop();
    if(gpio_get_level(clock_rx_pin)==0)
    header_detect_state = true;
    else
    {
        header_detect_state = false;
        gpio_set_intr_type(clock_rx_pin,GPIO_INTR_NEGEDGE);
        gpio_intr_enable(clock_rx_pin);
    }
    return true;
}
esp_err_t header_clock_init(uint64_t duration,int rx_pin)
{
    esp_err_t ret = ESP_OK;
    uint64_t header_alarm_count = 4*duration;
    clock_rx_pin = rx_pin;

    // 1. init the timer
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = CLOCK_TIMES * 1000 * 1000, // CLOCK_TIMESMHz
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer_header));

    // 2. set the alarm conut and reload count
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = header_alarm_count*CLOCK_TIMES,
        .flags.auto_reload_on_alarm = false,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer_header, &alarm_config));

    // 3. set the alarm callback function
    gptimer_event_callbacks_t cbs = {
        .on_alarm = header_start_receving,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer_header, &cbs, NULL));

    // 4. enable the timer.
    ESP_ERROR_CHECK(gptimer_enable(gptimer_header));

    header_clock_state=false;
    return ret;
}

void header_clock_start()
{
    gptimer_set_raw_count(gptimer_header,(uint64_t)0);
    header_clock_state = true;
    ESP_ERROR_CHECK(gptimer_start(gptimer_header));
}

void header_clock_stop()
{
    ESP_ERROR_CHECK(gptimer_stop(gptimer_header));
    header_clock_state = false;
    gptimer_set_raw_count(gptimer_header,(uint64_t)0);
}

bool header_clock_get_state()
{
    return header_clock_state;
}

bool get_header_detect_state()
{
    return header_detect_state;
}

void set_header_detect_state(bool state)
{
    header_detect_state = state;
}
 
