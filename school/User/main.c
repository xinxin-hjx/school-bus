/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MW8266D模块原子云连接实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "demo.h"
#include "./BSP/HW/hw.h"
#include "./BSP/DHT11/dht11.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/DMA/dma.h"
#include "./BSP/ADC/mq135.h"
#include "./BSP/OLED/OLED_Font.h"
#include "./BSP/AIRC/airc.h"
#include "./BSP/MP3/mp3.h"

/**
 * @brief       显示实验信息
 * @param       无
 * @retval      无
 */
void show_mesg(void)
{    
    
    /* 串口输出实验信息 */
    printf("\n");
    printf("********************************\r\n");
    printf("STM32\r\n");
    printf("ATK-MW8266D\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("KEY0: test key\r\n");
    printf("KEY1: switch atkcld mode\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
		usart2_init(9600);									/* 串口初始化为9600 */
    led_init();                         /* 初始化LED */
    key_init();
    airc_init();
	  beep_init();	/* 初始化按键 */   												
    show_mesg();	/* 显示实验信息 */
	  HW_Init();	
		MY_ADC_Init(ADC_CHANNEL_1, ADC_CHANNEL_2);
	  OLED_Init();
		dht11_init();	
    demo_run();                         /* 运行示例程序 */	 
}


