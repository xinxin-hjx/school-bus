#include "./BSP/MP3/mp3.h"
UART_HandleTypeDef g_uart2_handle;
uint16_t g_usart2_rx_sta = 0;
uint8_t g2_rx_buffer[RX2BUFFERSIZE];
void usart2_init(uint32_t baudrate)
{
    /*UART ≥ı ºªØ…Ë÷√*/
    g_uart2_handle.Instance = mp3_usart;                                       
    g_uart2_handle.Init.BaudRate = baudrate;                                 
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;                      
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;                           
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;                           
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      
    g_uart2_handle.Init.Mode = UART_MODE_TX_RX;
    //HAL_UART_Init(&g_uart2_handle);
if (HAL_UART_Init(&g_uart2_handle) != HAL_OK)  
    {  
        Error_Handler();  
    }	
  
    //HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)g2_rx_buffer, RX2BUFFERSIZE); 
}
 
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
    __HAL_RCC_USART2_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, mp3_TX_GPIO_PIN|mp3_RX_GPIO_PIN);
    HAL_NVIC_DisableIRQ(USART2_IRQn);
}

void print_bytes(const uint8_t *bytes, size_t size) {  
    for (size_t i = 0; i < size; i++) {  
        printf("%02X ", bytes[i]);  
    }  
   
}
void Error_Handler(void)
{
	while(1)
	{
		LED1(1);
		delay_ms(1000);
		LED1(0);
	
	}
}
void voice_select(int which )
{
	if(which == 1)
	{
		uint8_t voice1[] = {0x7E,0xFF,0x06,0x03,0x00,0x00,0x01,0xFE,0xF7,0xEF};
		HAL_UART_Transmit(&g_uart2_handle, voice1, sizeof(voice1), 0x0FFF);
	}
	else if(which == 2)
	{
		uint8_t voice1[] = {0x7E,0xFF,0x06,0x03,0x00,0x00,0x02,0xFE,0xF6 ,0xEF};
		HAL_UART_Transmit(&g_uart2_handle, voice1, sizeof(voice1), 0x0FFF);
	}
	else if(which == 4)
	{
		uint8_t voice1[] = {0x7E,0xFF,0x06,0x03,0x00,0x00,0x04,0xFE,0xF4 ,0xEF};
		HAL_UART_Transmit(&g_uart2_handle, voice1, sizeof(voice1), 0x0FFF);
	}
}

