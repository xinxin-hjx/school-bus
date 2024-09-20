#ifndef _MP3_H
#define _MP3_H
#include "./SYSTEM/sys/sys.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#define mp3_TX_GPIO_PROT GPIOA
#define mp3_TX_GPIO_PIN GPIO_PIN_2
#define mp3_RX_GPIO_PROT GPIOA
#define mp3_RX_GPIO_PIN GPIO_PIN_3
#define USART_U2_IRQn                       USART2_IRQn
#define mp3_RX_CLK_ENABLE() do{__HAL_RCC_GPIOA_CLK_ENABLE();} while(0)
#define mp3_TX_CLK_ENABLE() do{__HAL_RCC_GPIOA_CLK_ENABLE();} while(0)
#define USART_U2_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)
#define mp3_usart USART2
#define RX2BUFFERSIZE   1 
void MX_USART2_UART_Init(void);
void HAL_UART2_MspInit(UART_HandleTypeDef* uartHandle);
//void mp3_trans(void);
//void print_bytes(const uint8_t *bytes, size_t size);
extern UART_HandleTypeDef g_uart2_handle;
void usart2_init(uint32_t baudrate);
void Error_Handler(void);
void mp3_trans(void);
void voice_select(int which );

#endif


