//
// Created by Administrator on 11/30/2023.
//

#include "VLC_receiver.h"
#include "VLC_decoder.h"
#include "VLC_timer.h"

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"

extern uint8_t DETECT_DATA_FLAG;
extern uint8_t VLC_RX_DATA_BUFFER[RECEIVE_BYTES_LENGTH];
extern uint8_t VLC_RX_UART_BUFFER[RX_BUFFER_LENGTH];
extern uint8_t received_data[FRAME_LENGTH];


#define BIT_ADDR(byte_offset, bit_number) (volatile unsigned long*)(0x42000000+((byte_offset)<< 5) +(bit_number << 2))
#define GPIOA_OCTL_OFFSET ((GPIOA_BASE+0x14) - 0x40000000)
#define GPIOA_ISTAT_OFFSET ((GPIOA_BASE+0x10) - 0x40000000)

#define GPIOB_OCTL_OFFSET ((GPIOB_BASE+0x14) - 0x40000000)

#define PAout(n) (*(BIT_ADDR(GPIOA_OCTL_OFFSET,n)))
#define PAin(n) (*(BIT_ADDR(GPIOA_ISTAT_OFFSET,n)))

#define PBout(n) (*(BIT_ADDR(GPIOB_OCTL_OFFSET,n)))

#define SET_BIT_OF(v, bit, n) ((v)|(bit<<(n)))

#define PD1 6
#define WLED 15

static enum RECV_STATE state = VLC_IDLE;

void VLC_receiver_start() {
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart4, VLC_RX_UART_BUFFER, RX_BUFFER_LENGTH);
}

void VLC_receiver_receive(uint8_t *payload) {
    /** The data packet is wrong!**/
    if (VLC_RX_DATA_BUFFER[0] != 0xFF) {
        payload[0] = 0xFF;
    } else {
        VLC_decode(VLC_RX_DATA_BUFFER, payload);
    }
}


void VLC_Gpio_DoReceive() {
    VLC_udelay(VLC_BIT_DURATION * 3 ); // Discard the header left bits.
    uint8_t temp_bit = 0;
    for (int i = 0; i < RECEIVE_BYTES_LENGTH; i++) {
        VLC_RX_DATA_BUFFER[i] = 0;
        for (int j = 7; j >= 0; j--) {
            temp_bit = 0;
            /** 4 times oversampling **/
            for (int ZZZ = 0; ZZZ < 4; ZZZ++) {
                temp_bit += PAin(PD1);
                VLC_udelay(VLC_SAMPLING_INTERVAL);
            }
            temp_bit = temp_bit >= 2 ? 1 : 0;
            temp_bit = PAin(PD1);
            VLC_RX_DATA_BUFFER[i] = SET_BIT_OF(VLC_RX_DATA_BUFFER[i], temp_bit, j);
        }
    }
    VLC_decode(VLC_RX_DATA_BUFFER, received_data);
//    HAL_UART_Transmit(&huart1, received_data, 4, HAL_MAX_DELAY);
//    HAL_UART_Transmit(&huart1, "A", 1, HAL_MAX_DELAY);

    state = VLC_IDLE;
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}


/**
 * Gpio Rising Edge Interrupt Handler Function.
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    switch (state) {
        /** Detect the Message Header **/
        case VLC_IDLE: {
            if (PAin(PD1) == 1) {
                __HAL_TIM_SET_COUNTER(&htim4, 0);
                HAL_TIM_Base_Start_IT(&htim4);
                state = VLC_PRE;
            }
            break;
        }
        case VLC_PRE: {
            HAL_TIM_Base_Stop_IT(&htim4);
            __HAL_TIM_SET_COUNTER(&htim4, 0);
            state = VLC_IDLE;
            break;
        }
    }
}

/**
 * The timer callback is used to detect the header.
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == (&htim4)) {
        HAL_TIM_Base_Stop_IT(&htim4);
        HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        DETECT_DATA_FLAG = 1;
        state = VLC_RECEIVING;

        VLC_Gpio_DoReceive();


    }
}

