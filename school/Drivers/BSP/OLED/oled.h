#ifndef __OLED_H
#define __OLED_H

#include "stdlib.h"
#include "./SYSTEM/sys/sys.h"
#include "./BSP/OLED/OLED_Font.h" 
#include "./BSP/OLED/OLED_Font.h"
#include "./BSP/DHT11/dht11.h"

/*“˝Ω≈≈‰÷√*/
#define OLED_W_SCL(x)		do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (GPIO_PinState)(x));}while(0)
#define OLED_W_SDA(x)		do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState)(x));}while(0)

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t page,uint8_t list,uint8_t ch);
void OLED_ShowString(uint8_t page, uint8_t list, const char* str,uint8_t w,uint8_t h);
void OLED_Show(uint8_t page,uint8_t list,uint8_t *str,uint8_t w,uint8_t h);
void OLED_ShowPic(uint8_t page,uint8_t list,const uint8_t *pic,uint8_t w,uint8_t h);
void oled_dis_char(uint8_t page,uint8_t list,uint8_t ch,uint8_t w,uint8_t h);
void update_display();

#endif
