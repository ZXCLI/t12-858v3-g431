/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_adc.h"
#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_dma.h"

#include "stm32g4xx_ll_exti.h"

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
#define TEST2_Pin GPIO_PIN_13
#define TEST2_GPIO_Port GPIOC
#define TEST1_Pin GPIO_PIN_14
#define TEST1_GPIO_Port GPIOC
#define IRON_TEMP_Pin GPIO_PIN_1
#define IRON_TEMP_GPIO_Port GPIOA
#define AMBIENT_TEMP_Pin GPIO_PIN_2
#define AMBIENT_TEMP_GPIO_Port GPIOA
#define IRON_CURRENT_Pin GPIO_PIN_3
#define IRON_CURRENT_GPIO_Port GPIOA
#define FAN_CURRENT_Pin GPIO_PIN_4
#define FAN_CURRENT_GPIO_Port GPIOA
#define VBUS_Pin GPIO_PIN_5
#define VBUS_GPIO_Port GPIOA
#define IRON_ID_Pin GPIO_PIN_6
#define IRON_ID_GPIO_Port GPIOA
#define GUN_TEMP_Pin GPIO_PIN_7
#define GUN_TEMP_GPIO_Port GPIOA
#define GUN_SLEEP_Pin GPIO_PIN_0
#define GUN_SLEEP_GPIO_Port GPIOB
#define IRON_SLEEP_Pin GPIO_PIN_1
#define IRON_SLEEP_GPIO_Port GPIOB
#define SWITCH_Pin GPIO_PIN_2
#define SWITCH_GPIO_Port GPIOB
#define OLED_RES_Pin GPIO_PIN_10
#define OLED_RES_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_11
#define OLED_CS_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_12
#define OLED_DC_GPIO_Port GPIOB
#define OLED_SCK_Pin GPIO_PIN_13
#define OLED_SCK_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_14
#define BUZZER_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_15
#define OLED_SDA_GPIO_Port GPIOB
#define FAN_PWM_Pin GPIO_PIN_6
#define FAN_PWM_GPIO_Port GPIOC
#define IRON_PWM_Pin GPIO_PIN_8
#define IRON_PWM_GPIO_Port GPIOA
#define DIS_PWM_Pin GPIO_PIN_9
#define DIS_PWM_GPIO_Port GPIOA
#define GUN_PWM_Pin GPIO_PIN_10
#define GUN_PWM_GPIO_Port GPIOA
#define ENCODE_L_Pin GPIO_PIN_15
#define ENCODE_L_GPIO_Port GPIOA
#define ENCODE_R_Pin GPIO_PIN_10
#define ENCODE_R_GPIO_Port GPIOC
#define ENCODE_S_Pin GPIO_PIN_11
#define ENCODE_S_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_4
#define KEY1_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_5
#define KEY2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
