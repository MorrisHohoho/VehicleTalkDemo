/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENCODERA_Pin GPIO_PIN_1
#define ENCODERA_GPIO_Port GPIOA
#define ENCODERB_Pin GPIO_PIN_2
#define ENCODERB_GPIO_Port GPIOA
#define SERVO_Pin GPIO_PIN_7
#define SERVO_GPIO_Port GPIOA
#define MOTORA_Pin GPIO_PIN_11
#define MOTORA_GPIO_Port GPIOB
#define MOTORB_Pin GPIO_PIN_12
#define MOTORB_GPIO_Port GPIOB
#define WLED_Pin GPIO_PIN_15
#define WLED_GPIO_Port GPIOB
#define UART4_TX_Pin GPIO_PIN_10
#define UART4_TX_GPIO_Port GPIOC
#define UART4_RX_Pin GPIO_PIN_11
#define UART4_RX_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */
#define PAYLOAD_LENGTH 59 //The original data length, in bytes. i.e.: "abcd" length is 4.
#define FRAME_LENGTH  (PAYLOAD_LENGTH+1) //The frame data length (add 1 bytes PSN), in bytes. i.e. : 1+"abdc" length is 5.

#define VLC_BUAD_RATE 115200
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
