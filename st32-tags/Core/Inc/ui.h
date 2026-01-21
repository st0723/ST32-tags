#ifndef __UI_H__
#define __UI_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "key.h"

enum
{
    UISTATE_RUNNING,            //正常运行状态
};
typedef BYTE ENUM_UISTATE;

enum
{
    OPMODE_MAIN,                // 主操作模式
	ENUM_OPMODE_COUNT,          // 操作模式数量
};
typedef BYTE ENUM_OPMODE;

enum
{
    FLAG_NONE,              	// 无操作或默认状态
};
typedef BYTE ENUM_OPMODE_FLAG;

typedef void (*KEY_EVENT_CALLBACK)(KEY_CODE code);
typedef void (*UI_INIT_CALLBACK)(ENUM_OPMODE srcMode, void *param, ENUM_OPMODE_FLAG flag);
typedef struct
{ 
    ENUM_OPMODE         OpMode;
    KEY_EVENT_CALLBACK  KeyEvent;
    UI_INIT_CALLBACK    Init;
}const ST_UI_HANDLE;


BaseType_t UI_Init(void);

#ifdef __cplusplus
}
#endif

#endif 


