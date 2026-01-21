#include "key.h"

#define KEY_WRITE_TIMEOUT 3000

static TaskHandle_t  KeyScanTaskHandle 		= NULL;
static QueueHandle_t KeyBufferQueueHandle 	= NULL;
static SemaphoreHandle_t _hKeyScanMutex 	= NULL;

struct IO_PORT
{                                            
        GPIO_TypeDef *GPIO_x;                 
        unsigned short GPIO_pin;
};
static struct IO_PORT KEY_IN[KEYPAD_ROWS] = {
        {KEYPAD_ROW1_PORT, KEYPAD_ROW1_PIN},
		{KEYPAD_ROW2_PORT, KEYPAD_ROW2_PIN},
        {KEYPAD_ROW3_PORT, KEYPAD_ROW3_PIN},
		{KEYPAD_ROW4_PORT, KEYPAD_ROW4_PIN}, 		
};

typedef struct {
    uint8_t key_value;
} KeyMsg_t;

unsigned char key[KEYPAD_ROWS];

static void KEYPAD_Scan_Task(void *argument);
static void DetectKeyPress(uint8_t *key);

void KEY_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    KEYPAD_ROW1_GPIO_CLK_ENABLE();        
    KEYPAD_ROW2_GPIO_CLK_ENABLE();
	KEYPAD_ROW3_GPIO_CLK_ENABLE();
	KEYPAD_ROW4_GPIO_CLK_ENABLE();
	
    GPIO_InitStruct.Mode 	= GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull 	= GPIO_PULLUP;

	GPIO_InitStruct.Pin 	= KEYPAD_ROW1_PIN;
    HAL_GPIO_Init(KEYPAD_ROW1_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin 	= KEYPAD_ROW2_PIN;
    HAL_GPIO_Init(KEYPAD_ROW2_PORT, &GPIO_InitStruct);	
	GPIO_InitStruct.Pin 	= KEYPAD_ROW3_PIN;
    HAL_GPIO_Init(KEYPAD_ROW3_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin 	= KEYPAD_ROW4_PIN;
    HAL_GPIO_Init(KEYPAD_ROW4_PORT, &GPIO_InitStruct);
}

/* 扫描按键任务 */
static void KEYPAD_Scan_Task(void *argument)
{
	unsigned char i;
    while(1)
    {
        for(i = 0; i < KEYPAD_ROWS; i++)            
        {
			if(HAL_GPIO_ReadPin(KEY_IN[i].GPIO_x, KEY_IN[i].GPIO_pin) == 0)
			{
				key[i] = 1;
			}
			else
			{
				key[i] = 0;
			}
        }
		DetectKeyPress(key);
        vTaskDelay(10);
    }
}

static void DetectKeyPress(uint8_t *key)
{
    static uint8_t last_key[KEYPAD_ROWS] = {0};
	if(xSemaphoreTake(_hKeyScanMutex, KEY_WRITE_TIMEOUT) != pdTRUE)
    {
        return;
    }    
    for(int i = 0; i < KEYPAD_ROWS; i++) 
	{
		if(key[i] == 1 && last_key[i] == 0) 
		{
			KeyMsg_t key_msg;
			key_msg.key_value = i + 1;
			xQueueSend(KeyBufferQueueHandle, &key_msg, 0);
			
		}
		last_key[i] = key[i];
    }
	xSemaphoreGive(_hKeyScanMutex);
}

// 初始化按键矩阵
BaseType_t KEYPAD_Init(void)
{   
	if (KeyBufferQueueHandle == NULL) 
	{
		KeyBufferQueueHandle = xQueueCreate(10, sizeof(KeyMsg_t));	// 创建消息队列
	}
	if(_hKeyScanMutex == NULL)
    {
        _hKeyScanMutex = xSemaphoreCreateMutex();					// 创建信号量
    }
	KEY_GPIO_Init();
	/* 创建按键扫描任务 */
	xTaskCreate((TaskFunction_t )KEYPAD_Scan_Task,  	/* 任务入口函数 */
				(const char*    )"KeyScan",				/* 任务名字 */
				(uint16_t       )256,  					/* 任务栈大小 */
				(void*          )NULL,					/* 任务入口函数参数 */
				(UBaseType_t    )3, 					/* 任务的优先级 */
				(TaskHandle_t*  )&KeyScanTaskHandle);	/* 任务控制块指针 */ 	    
    return pdPASS;
} 

void Key_BufferClear(void)
{
    BYTE code;
    while(xQueueReceive(KeyBufferQueueHandle, &code, 0));
}

KEY_CODE Key_BufferRead(UINT32 timeout)
{
    BYTE code = NULL;
    if(xQueueReceive(KeyBufferQueueHandle, &code, timeout))
        return code;
    else        
        return NULL;
	
}


