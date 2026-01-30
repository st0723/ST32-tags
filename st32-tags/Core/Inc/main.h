/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include <stdarg.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

typedef uint8_t 	UINT8, BYTE;
typedef uint16_t 	UINT16;
typedef uint32_t 	UINT32;
typedef int8_t 		INT8;
typedef int16_t 	INT16;
typedef int32_t 	INT32;
typedef BYTE 		BOOL;


typedef struct
{
	BOOL Beep_Switch;					// 蜂鸣器开关
	BOOL Photoelectric_Switch;			// 电眼开关
	uint8_t Mode_Selection;				// 模式选择
	uint8_t Speed_Grade;				// 自动模式下的速度等级
	BOOL Missing_detection_switch;		// 漏标检测开关
	// 漏标后自动补一个标
	// 漏标检测时间
	// 标卷缺标报警个数
	// 电机连续运转时间
	// 报警状态下能否贴标
	// 通讯地址
	// 标签数预警
	// 恢复出厂设置
	// 电眼分辨率
	// 电眼实时信号强度
	// 编码器脉冲数
	// 编码器轮/辊直径
	// 速度系数
	// 漏标信号脉宽
	// 延时出标时间
	// 压标时间调整
	// 速度校正系数
	// 当前校正后速度
	// 漏标检测时间系数
	// 漏标检测时间显示
}ST_CONFIGURATION_INFO;
extern ST_CONFIGURATION_INFO ConfigInfo;

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
