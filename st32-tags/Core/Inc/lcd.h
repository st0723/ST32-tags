#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void LCD_Init(void);
BYTE lcdPutShortText(const char *text, ...);

#ifdef __cplusplus
}
#endif

#endif 


