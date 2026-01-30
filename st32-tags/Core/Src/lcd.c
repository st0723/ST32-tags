#include "lcd.h"
#include "usart.h"

#define LCD_WRITE_TIMEOUT 3000

static SemaphoreHandle_t _hSendMutex 	= NULL;
static SemaphoreHandle_t _hLcdMutex 	= NULL;

static HAL_StatusTypeDef lcdSendByte(uint8_t Byte) 
{
	HAL_StatusTypeDef status;
	if(xSemaphoreTake(_hSendMutex, LCD_WRITE_TIMEOUT) != pdTRUE)
	{
		return HAL_TIMEOUT;
	}
	status = HAL_UART_Transmit(&huart3, &Byte, 1, 100);
	xSemaphoreGive(_hSendMutex);
	return status;
}

static HAL_StatusTypeDef lcdSendData(uint8_t* Data) 
{
	lcdSendByte(0x08);
	for(uint8_t i = 0; i < 5; i++)
	{
		lcdSendByte(Data[i]);
	}
	vTaskDelay(3);
	lcdSendByte(0x18);
	lcdSendByte(0x1C);
}

uint8_t lcdTransform(uint8_t showChar) 
{
	uint8_t data = 0xff;
	switch(showChar)
	{
		case '0':		data = 0x3F;	break;	
		case '1':		data = 0x06;	break;
		case '2':		data = 0x5B;	break;
		case '3':		data = 0x4F;	break;
		case '4':		data = 0x66;	break;
		case '5':		data = 0x6D;	break;
		case '6':		data = 0x7D;	break;
		case '7':		data = 0x07;	break;
		case '8':		data = 0x7F;	break;
		case '9':		data = 0x6F;	break;
		case 'A':		data = 0x77;	break;
		case 'B':		data = 0x7C;	break;
		case 'C':		data = 0x39;	break;
		case 'D':		data = 0x5E;	break;
		case 'E':		data = 0x79;	break;
		case 'F':		data = 0x71;	break;
		case 'H':       data = 0x76;    break;
		case 'P':       data = 0x73;    break;
	}
	return data;
};

void shiftArrayBackward(uint8_t* arr, int size, int positions) 
{
    if (positions <= 0 || positions >= size) 
		return;
    for (int i = size - 1; i >= positions; i--) 		// 从后向前移动
	{
        arr[i] = arr[i - positions];
    }
    for (int i = 0; i < positions; i++) 				// 将前面的位置设为0（或其他默认值）
	{
        arr[i] = 0;
    }
}

BYTE lcdPutShortText(const char *text, ...)
{
    if(xSemaphoreTake(_hLcdMutex, LCD_WRITE_TIMEOUT) != pdTRUE)
    {
        return 0;
    }
    char buf[8] = "";
	uint8_t i = 0;
	uint8_t DataBuf[5] = {0};
    va_list args;
    va_start(args, text);
    vsnprintf(buf, 8, text, args);
    va_end(args);
    BYTE *ptr = (BYTE *)buf;
    while((*ptr != '\0') && (*ptr != '\n'))
    {
		BYTE code = *ptr++;
		if(code == '.')
		{
			DataBuf[i - 1] |= 0x80;		
		}
		else
		{
			DataBuf[i++] = lcdTransform(code);
		}
    }	
	shiftArrayBackward(DataBuf,5,5-i);
	lcdSendData(DataBuf);
    xSemaphoreGive(_hLcdMutex);
}

void LCD_Init(void) 
{
	if(_hLcdMutex == NULL)
	{
		_hLcdMutex = xSemaphoreCreateMutex();
	}
	if(_hSendMutex == NULL)
	{
		_hSendMutex = xSemaphoreCreateMutex();
	}
} 
