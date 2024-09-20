/**
 ****************************************************************************************************
 * @file        demo.c
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

#include "demo.h"
#include "./BSP/ATK_MW8266D/atk_mw8266d.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/MQTT/mqtt.h"
#include "./BSP/DHT11/dht11.h"
#include "./BSP/MP3/mp3.h"

#define DEMO_WIFI_SSID          "pi"
#define DEMO_WIFI_PWD           "88888888"
#define DEMO_ONENET_SEVER_IP		"mqtts.heclouds.com"
#define DEMO_ONENET_SEVER_PORT  (1883)

#define AT_CMD_BUF 							"{\\\"id\\\":123\\\,\\\"dp\\\":{\\\"wendu\\\":[{\\\"v\\\": %d}]\\\,\\\"shidu\\\": [{\\\"v\\\":%d}]\\\,\\\"air\\\": [{\\\"v\\\":%0.1f}]}}"

/**
 * @brief       显示IP地址
 * @param       无
 * @retval      无
 */
static void demo_show_ip(char *buf)
{
    printf("IP: %s\r\n", buf);
    
}



/**
 * @brief       按键2功能，切换OneNet连接状态
 * @param       is_atkcld: 0，未连接OneNet
 *                         1，已连接OneNet
 * @retval      无
 */
static void demo_key2_fun(uint8_t *is_atkcld)
{
    uint8_t ret;
    
    if (*is_atkcld == 0)
    {
        /* 连接OneNet */
        ret = atk_mw8266d_connect_onenet_server(DEMO_ONENET_SEVER_IP, DEMO_ONENET_SEVER_PORT);
        if (ret == 0)
        {
            *is_atkcld = 1;
            printf("Connect to OneNet cloud!\r\n");
					
        }
        else
        {
            printf("Error to connect OneNet cloud!\r\n");
        }
    }
    else
    {
        /* 断开OneNet连接 */
        ret = atk_mw8266d_disconnect_onenet_server();
				if(ret==0)
				{
					*is_atkcld = 0;
					printf("Disconnect to OneNet cloud success!\r\n");
				}
				else
				{
					printf("Disconnect to OneNet cloud fail!\r\n");
				}
		}
}


/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
//AT+MQTTPUB=0,"$sys/w6fFrZDTha/test/dp/post/json","{\"id\":123\,\"dp\":{\"temp\":[{\"v\": 40}]\,\"hum\": [{\"v\":40}]}}",0,0

static void sensor_process(void)
{
	  ADC_Filter();
		uint8_t temp,humi;
	  float air;
		char buf[MQTT_MSG_MAX_SIZE];	  
		dht11_read_data(&temp, &humi);
	  air =Get_MQ135Value(10);
	  sprintf(buf,AT_CMD_BUF,temp,humi,air);
		demo_msg_pub(PubTopic,buf,PubQos);
	update_display();
	if(temp >=30)
	{
		airc(1);	
	}
	else if(temp < 30)
	{
		airc(0);
	}
	delay_ms(1000);

}  

void demo_run(void)
{
    uint8_t ret;
    char ip_buf[16];
    uint8_t key;
    uint8_t is_atkcld = 0;
	  uint8_t substatus = 1;
    
    /* 初始化ATK-MW8266D */
    ret = atk_mw8266d_init(115200);
    if (ret != 0)
    {
        printf("ATK-MW8266D init failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
		
    /* 连接路由器 */
    printf("Joining to AP...\r\n");
		
    ret  = atk_mw8266d_restore();                               /* 恢复出厂设置 */
    printf("wifi restore ret:%d\r\n",ret);
		
    ret += atk_mw8266d_at_test();                               /* AT测试 */
	 	printf("wifi test ret:%d\r\n",ret);
		
    ret += atk_mw8266d_set_mode(1);                             /* Station模式 */
		printf("wifi set station ret:%d\r\n",ret);
		
    ret += atk_mw8266d_sw_reset();                              /* 软件复位 */
		printf("wifi rst ret:%d\r\n",ret);
		delay_ms(100); 
	
    ret += atk_mw8266d_ate_config(0);                           /* 关闭回显功能 */
		printf("close show ret:%d\r\n",ret);
		
    ret += atk_mw8266d_join_ap(DEMO_WIFI_SSID, DEMO_WIFI_PWD);  /* 连接WIFI */
		printf("wifi connect ret:%d\r\n",ret);
		
    ret += atk_mw8266d_get_ip(ip_buf);                          /* 获取IP地址 */
		printf("wifi get ip ret:%d\r\n",ret);
		
		
    if (ret != 0)
    {
        printf("Error to join ap!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    demo_show_ip(ip_buf);			
		
		/* 配置MQTT信息 */
		ret = demo_mqtt_userconf();																   /* 配置MQTT用户信息 */
		printf("mqtt user msg config ret:%d\r\n",ret); 		
		ret+= demo_mqtt_connconf();																	 /* 配置MQTT连接信息 */
		printf("mqtt connect msg config ret:%d\r\n",ret);
		
		if (ret != 0)
    {
        printf("Mqtt init fail!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }	

    /* 重新开始接收新的一帧数据 */
    atk_mw8266d_uart_rx_restart();
		int C=0;
		int try=0;
			
    while (1)
    {			  
        key = key_scan(0);						
        switch (key)
        {
            case KEY1_PRES:
            {
						 	voice_select(4);				
							break;
            }
						case KEY2_PRES:
            {
                /* 切换onenet连接状态 */ 
								demo_key2_fun(&is_atkcld);
								if(is_atkcld)
								{
									substatus = demo_topic_sub(PubResultTopic);
								}
								else
								{
									substatus = 1;
								}
								try++;
								
                break;
            }
						case KEY0_PRES:
            {
							char xc[MQTT_MSG_MAX_SIZE];							
							if(C==0)
							{
								printf("校车已出发 %d\r\n",C);																
								sprintf(xc,AT_XC_BUF,C);
								demo_msg_pub(PubTopic,xc,PubQos);
								C++;
								
							}
							else if(C==1)
							{
								printf("校车已到达 %d\r\n",C);
								sprintf(xc,AT_XC_BUF,C);
								demo_msg_pub(PubTopic,xc,PubQos);
								C=0;
								
							}
							try=0;
							delay_ms(1000);					
								try++;              
                break;
            }
            default:
            {
                break;
            }
        }
				
				if(try==1)
			{
				HW_run( );
				sensor_process();								
				delay_ms(1000);
			}
				 

        /* 接收自OneNet的数据到串口调试助手 */
        demo_receive_onenet_data(is_atkcld,substatus);
        
        //delay_ms(10);
    }
}


