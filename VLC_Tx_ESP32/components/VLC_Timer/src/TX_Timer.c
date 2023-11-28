#include "TX_Timer.h"

#include "driver/gptimer.h"



static gptimer_handle_t TX3_gptimer = NULL;


void TX3_gpitimer_init(){

    gptimer_config_t timer_config={
        .clk_src=GPTIMER_CLK_SRC_DEFAULT,
        .direction=GPTIMER_COUNT_UP,
        .resolution_hz = 1*1000*1000,
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config,&TX3_gptimer));
    ESP_ERROR_CHECK(gptimer_enable(TX3_gptimer));
}


/**
 * @brief Helper function, start the timer
*/
static void TX3_gpitmer_start(){
    ESP_ERROR_CHECK(gptimer_set_raw_count(TX3_gptimer,0));
    ESP_ERROR_CHECK(gptimer_start(TX3_gptimer));
}
/**
 * @brief Helper fucntion, stop the timer
*/
static void TX3_gpitmer_stop(){
    ESP_ERROR_CHECK(gptimer_stop(TX3_gptimer));
    ESP_ERROR_CHECK(gptimer_set_raw_count(TX3_gptimer,0));
}

void TX3_udelay(uint64_t us){
    TX3_gpitmer_start();
    uint64_t cur = 0;
    while(cur<us){
        ESP_ERROR_CHECK(gptimer_get_raw_count(TX3_gptimer,&cur));
    }
    TX3_gpitmer_stop();
}