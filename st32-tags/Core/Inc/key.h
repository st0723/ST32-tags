#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define KEYPAD_ROWS    4

enum
{
	KEY_CODE_NULL   = 0x00,         // 空按键值，通常用于表示无效或没有按键被按下
	KEY_EX_MODE     = 0x01,			// 菜单键
    KEY_EX_UP		= 0x02,			// 上箭头键
    KEY_EX_DOWN		= 0x03,         // 下箭头键
	KEY_EX_ENTER	= 0x04,         // 确认键
};
typedef BYTE KEY_CODE;

// 行引脚配置
#define KEYPAD_ROW1_PORT    GPIOB
#define KEYPAD_ROW1_PIN     GPIO_PIN_3
#define KEYPAD_ROW1_GPIO_CLK_ENABLE()		do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define KEYPAD_ROW2_PORT    GPIOB
#define KEYPAD_ROW2_PIN     GPIO_PIN_4
#define KEYPAD_ROW2_GPIO_CLK_ENABLE()		do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define KEYPAD_ROW3_PORT    GPIOB
#define KEYPAD_ROW3_PIN     GPIO_PIN_5
#define KEYPAD_ROW3_GPIO_CLK_ENABLE()		do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define KEYPAD_ROW4_PORT    GPIOB
#define KEYPAD_ROW4_PIN     GPIO_PIN_6
#define KEYPAD_ROW4_GPIO_CLK_ENABLE()		do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

BaseType_t KEYPAD_Init(void);
void Key_BufferClear(void);
KEY_CODE Key_BufferRead(UINT32 timeout);

#ifdef __cplusplus
}
#endif

#endif 


