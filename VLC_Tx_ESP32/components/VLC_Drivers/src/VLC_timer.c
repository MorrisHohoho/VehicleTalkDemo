#include "VLC_timer.h"
#include "VLC_parameters.h"
#include "rom/ets_sys.h"

gptimer_handle_t VLC_timer_rx1_handler = NULL;
gptimer_handle_t VLC_timer_rx2_handler = NULL;

gptimer_handle_t VLC_timer_general = NULL;

void __attribute__((weak)) VLC_timer_PeriodElapsedCallback(gptimer_handle_t timer, void *arg) {}

esp_err_t VLC_timer_rx1N2_init(uint64_t count)
{
    esp_err_t ret = ESP_OK;
    uint64_t header_alarm_count = VLC_DATA_HEDAER_HIGH_BITS_LENGTH * count;

    // 1. init the timer
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        // VLC_TIMER_RESOULUTION MHz
        .resolution_hz = VLC_TIMER_RESOULUTION * 1000 * 1000,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &VLC_timer_rx1_handler));
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &VLC_timer_rx2_handler));

    // 2. set the alarm conut and reload count
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = header_alarm_count * VLC_TIMER_RESOULUTION,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(VLC_timer_rx1_handler, &alarm_config));
    ESP_ERROR_CHECK(gptimer_set_alarm_action(VLC_timer_rx2_handler, &alarm_config));

    // 3. set the alarm callback function

    gptimer_event_callbacks_t cbs = {
        .on_alarm = VLC_timer_PeriodElapsedCallback,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(VLC_timer_rx1_handler, &cbs, NULL));
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(VLC_timer_rx2_handler, &cbs, NULL));

    // 4. enable the timer.
    ESP_ERROR_CHECK(gptimer_enable(VLC_timer_rx1_handler));
    ESP_ERROR_CHECK(gptimer_enable(VLC_timer_rx2_handler));

    return ret;
}

void __attribute__((weak)) VLC_timer_generalCallback(gptimer_handle_t timer, void *arg) {}

esp_err_t VLC_timer_general_init(uint64_t count, bool is_wallclock)
{
    esp_err_t ret = ESP_OK;
    uint64_t header_alarm_count = count;

    // 1. init the timer
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        // VLC_TIMER_RESOULUTION MHz
        .resolution_hz = 1 * 1000 * 1000, // 1us per count
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &VLC_timer_general));

    if (!is_wallclock)
    {
        // 2. set the alarm conut and reload count
        gptimer_alarm_config_t alarm_config = {
            .alarm_count = header_alarm_count * VLC_TIMER_RESOULUTION,
            .flags.auto_reload_on_alarm = true,
        };
        ESP_ERROR_CHECK(gptimer_set_alarm_action(VLC_timer_general, &alarm_config));

        // 3. set the alarm callback function

        gptimer_event_callbacks_t cbs = {
            .on_alarm = VLC_timer_generalCallback,
        };
        ESP_ERROR_CHECK(gptimer_register_event_callbacks(VLC_timer_general, &cbs, NULL));
    }

    // 4. enable the timer.
    ESP_ERROR_CHECK(gptimer_enable(VLC_timer_general));

    // 5. Start the timer.
    ESP_ERROR_CHECK(gptimer_start(VLC_timer_general));

    return ret;
}


uint32_t get_VLC_general_timer_count()
{
    uint32_t count = 0;
    gptimer_get_raw_count(VLC_timer_general, &count);
    return count;
}
