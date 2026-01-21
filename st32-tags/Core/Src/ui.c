#include "ui.h"
#include "ui_main.h"

static TaskHandle_t  UIEventTaskHandle 		= NULL;
static SemaphoreHandle_t _hUIMutexMutex 	= NULL;

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
};

//用户界面按键事件任务
static void UIEventTask(void const *param)
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
		_hUIMutexMutex = xSemaphoreCreateMutex();		// 创建信号量
	}	
	xTaskCreate((TaskFunction_t )UIEventTask,  			/* 任务入口函数 */
				(const char*    )"UIKeyEventTask",		/* 任务名字 */
				(uint16_t       )512,  					/* 任务栈大小 */
				(void*          )NULL,					/* 任务入口函数参数 */
				(UBaseType_t    )1, 					/* 任务的优先级 */
				(TaskHandle_t*  )&UIEventTaskHandle);	/* 任务控制块指针 */
			
    return pdPASS;
}


