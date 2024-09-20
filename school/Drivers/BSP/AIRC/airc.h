#ifndef _AIRC_H
#define _AIRC_H
#include "./SYSTEM/sys/sys.h"
#include "./BSP/DHT11/dht11.h"
#define airc_PORT GPIOB
#define airc_PIN  GPIO_PIN_2
#define airc_CLK_ENABLE() do{__HAL_RCC_GPIOB_CLK_ENABLE();} while(0)
#define airc(x)   do{ x ?\
	HAL_GPIO_WritePin(airc_PORT, airc_PIN, GPIO_PIN_SET) : \
	HAL_GPIO_WritePin(airc_PORT, airc_PIN, GPIO_PIN_RESET); \
                  }while(0) 
								
void airc_init(void);
#endif
									
			


									