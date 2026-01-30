#include "ui.h"
#include "ui_main.h"
#include "ui_mode.h"

static TaskHandle_t  		UIKeyEventTaskHandle 	= NULL;
static SemaphoreHandle_t 	_hUIMutexMutex 			= NULL;
static SemaphoreHandle_t 	UIMutexHandle			= NULL;
typedef struct
{
    ENUM_UISTATE State;               		// 当前UI的状态
    ENUM_OPMODE  CurrentMode;          		// 当前的操作模式
    ENUM_OPMODE  LastMode;             		// 上一次的操作模式
    UINT16       IdleTimeCounter;      		// UI的空闲计时器
} ST_UI_STATUS;

static ST_UI_STATUS UIStatus = {
    .State              = UISTATE_RUNNING,
    .CurrentMode        = OPMODE_MAIN,
    .LastMode           = OPMODE_MAIN,
};

ST_UI_HANDLE               *OpmodeTable[ENUM_OPMODE_COUNT] =
{
    &UIHandle_Main, 
	&UIHandle_MODEConfig,
};



/*更改用户界面*/
void UI_OpModeChange(ENUM_OPMODE dstMode, void *param)
{
    BaseType_t status = xSemaphoreTake(UIMutexHandle, 0);
    Key_BufferClear();
	UIStatus.LastMode = UIStatus.CurrentMode;
    UIStatus.CurrentMode = dstMode;
    OpmodeTable[UIStatus.CurrentMode]->Init(UIStatus.LastMode, param, FLAG_NONE);
    if(status == pdTRUE)
    {
        xSemaphoreGive(UIMutexHandle);
    }
}


//用户界面按键事件任务
static void UIKeyEventTask(void const *param)
{
	KEY_CODE code = NULL;	
	OpmodeTable[UIStatus.CurrentMode]->Init(0, NULL, FLAG_NONE);
	while(1)
    {
		code = Key_BufferRead(100);
		if(code != NULL)
        {
			if(xSemaphoreTake(_hUIMutexMutex, 1000) == pdTRUE)
			{
				switch(UIStatus.State)
                {
                    default:
                    case UISTATE_RUNNING:
                        OpmodeTable[UIStatus.CurrentMode]->KeyEvent(code);
                        break;
                }
				xSemaphoreGive(_hUIMutexMutex);
			}
		}
		vTaskDelay(10);
	}
}

BaseType_t UI_Init(void)
{
	if(_hUIMutexMutex == NULL)
	{
		_hUIMutexMutex = xSemaphoreCreateMutex();
	}
	if(UIMutexHandle == NULL)
	{
		UIMutexHandle  = xSemaphoreCreateMutex();
	}
	xTaskCreate((TaskFunction_t )UIKeyEventTask,  			/* 任务入口函数 */
				(const char*    )"UIKeyEventTask",			/* 任务名字 */
				(uint16_t       )256,  						/* 任务栈大小 */
				(void*          )NULL,						/* 任务入口函数参数 */
				(UBaseType_t    )1, 						/* 任务的优先级 */
				(TaskHandle_t*  )&UIKeyEventTaskHandle);	/* 任务控制块指针 */
    return pdPASS;
}


