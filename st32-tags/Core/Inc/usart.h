#ifndef __USART_H__
#define __USART_H__
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;


#define USART2_RX_LEN 50
extern uint8_t USART2_RX_BUF[USART2_RX_LEN];
extern uint16_t USART2_RX_STA; 

int fputc(int ch, FILE *f);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

