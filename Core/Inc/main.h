/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stdio.h"
#include "MyTask.h"
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
#define F3_Pin GPIO_PIN_2
#define F3_GPIO_Port GPIOE
#define F7_Pin GPIO_PIN_3
#define F7_GPIO_Port GPIOE
#define F8_Pin GPIO_PIN_4
#define F8_GPIO_Port GPIOE
#define F6_Pin GPIO_PIN_5
#define F6_GPIO_Port GPIOE
#define F4_Pin GPIO_PIN_6
#define F4_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_1
#define KEY2_GPIO_Port GPIOC
#define KEY3_Pin GPIO_PIN_2
#define KEY3_GPIO_Port GPIOC
#define L4_Pin GPIO_PIN_5
#define L4_GPIO_Port GPIOC
#define L1_Pin GPIO_PIN_0
#define L1_GPIO_Port GPIOB
#define L1_EXTI_IRQn EXTI0_IRQn
#define L6_Pin GPIO_PIN_1
#define L6_GPIO_Port GPIOB
#define L3_Pin GPIO_PIN_2
#define L3_GPIO_Port GPIOB
#define R8_Pin GPIO_PIN_8
#define R8_GPIO_Port GPIOE
#define L5_Pin GPIO_PIN_9
#define L5_GPIO_Port GPIOE
#define L8_Pin GPIO_PIN_10
#define L8_GPIO_Port GPIOE
#define L7_Pin GPIO_PIN_11
#define L7_GPIO_Port GPIOE
#define R6_Pin GPIO_PIN_12
#define R6_GPIO_Port GPIOB
#define R7_Pin GPIO_PIN_14
#define R7_GPIO_Port GPIOB
#define R4_Pin GPIO_PIN_15
#define R4_GPIO_Port GPIOB
#define R5_Pin GPIO_PIN_8
#define R5_GPIO_Port GPIOD
#define R3_Pin GPIO_PIN_9
#define R3_GPIO_Port GPIOD
#define R2_Pin GPIO_PIN_10
#define R2_GPIO_Port GPIOD
#define R1_Pin GPIO_PIN_11
#define R1_GPIO_Port GPIOD
#define M1_Pin GPIO_PIN_6
#define M1_GPIO_Port GPIOC
#define M2_Pin GPIO_PIN_8
#define M2_GPIO_Port GPIOC
#define KEY_Pin GPIO_PIN_15
#define KEY_GPIO_Port GPIOA
#define F5_Pin GPIO_PIN_3
#define F5_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define F2_Pin GPIO_PIN_0
#define F2_GPIO_Port GPIOE
#define F1_Pin GPIO_PIN_1
#define F1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
