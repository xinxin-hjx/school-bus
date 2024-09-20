/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MW8266Dģ��ԭ��������ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
 * @brief       ��ʾʵ����Ϣ
 * @param       ��
 * @retval      ��
 */
void show_mesg(void)
{    
    
    /* �������ʵ����Ϣ */
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
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
		usart2_init(9600);									/* ���ڳ�ʼ��Ϊ9600 */
    led_init();                         /* ��ʼ��LED */
    key_init();
    airc_init();
	  beep_init();	/* ��ʼ������ */   												
    show_mesg();	/* ��ʾʵ����Ϣ */
	  HW_Init();	
		MY_ADC_Init(ADC_CHANNEL_1, ADC_CHANNEL_2);
	  OLED_Init();
		dht11_init();	
    demo_run();                         /* ����ʾ������ */	 
}


