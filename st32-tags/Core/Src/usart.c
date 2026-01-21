#include "usart.h"


UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

uint8_t USART2_RX_BUF[USART2_RX_LEN];     	// 定义接收数组
uint16_t USART2_RX_STA = 0;     			// 第15bit表示一帧数据接收完成，第14~0位表示接收到的数据量


int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
	return ch;
}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_USART2_UART_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_UART_Receive_DMA(&huart2, USART2_RX_BUF, USART2_RX_LEN);  	// 启动DMA接收
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);              		// 使能空闲中断
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(uartHandle->Instance==USART1)
	{
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if(uartHandle->Instance==USART2)
	{
		/* USART2 clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		/* USART2 DMA Init */
		/* USART2_RX Init */			
		hdma_usart2_rx.Instance = DMA1_Channel6;
		hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
		hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
		{
		  Error_Handler();
		}

		__HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);
		
		HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
	if(uartHandle->Instance==USART1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
	}
	else if(uartHandle->Instance==USART2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_USART2_CLK_DISABLE();

		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
		
		/* USART2 DMA DeInit */
		HAL_DMA_DeInit(uartHandle->hdmarx);
	}
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void USART2_IRQHandler(void)
{

   if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)  					// 空闲中断标记被置位
   {   
       __HAL_UART_CLEAR_IDLEFLAG(&huart2);  									// 清除中断标记
       HAL_UART_DMAStop(&huart2);           									// 停止DMA接收	   
       USART2_RX_STA = USART2_RX_LEN - __HAL_DMA_GET_COUNTER(huart2.hdmarx);  	// 总数据量减去未接收到的数据量为已经接收到的数据量
       USART2_RX_BUF[USART2_RX_STA] = 0;  										// 添加结束符
       USART2_RX_STA |= 0X8000;         										// 标记接收结束
   HAL_UART_Transmit(&huart1,USART2_RX_BUF,USART2_RX_STA&0x0FFF,100);
       HAL_UART_Receive_DMA(&huart2, USART2_RX_BUF, USART2_RX_LEN);  			// 重新启动DMA接收
   }
}
