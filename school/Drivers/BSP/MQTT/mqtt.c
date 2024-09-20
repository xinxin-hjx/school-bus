#include "./BSP/MQTT/mqtt.h"
#include "stdlib.h"


//AT+MQTTPUB=0,"$sys/w6fFrZDTha/test/dp/post/json","{\"id\":123\,\"dp\":{\"temp\":[{\"v\": 40}]\,\"hum\": [{\"v\":40}]}}",0,0

uint8_t demo_msg_pub(uint8_t pub_topic[MQTT_MSG_MID_SIZE] ,char *buf ,uint8_t qos)
{
	
		int ret;
    char cmd[MQTT_MSG_MAX_SIZE];

    sprintf(cmd, "AT+MQTTPUB=%d,\"%s\",\"%s\",%d,%d", LinkID,pub_topic ,buf ,qos ,0);
		
	  printf("pub msg:%s\r\n",cmd);
	
    ret = atk_mw8266d_send_at_cmd(cmd, "OK", 1500);
		
		return ret;
}

//+MQTTSUBRECV:0,"$sys/w6fFrZDTha/test/dp/post/json/accepted",10,{"id":123}
uint8_t demo_topic_sub(char *topic)
{
	uint8_t ret;
  char cmd[MQTT_MSG_HIG_SIZE];
    
	//AT+MQTTSUB=<LinkID>,<"topic">,<qos>
   sprintf(cmd, "AT+MQTTSUB=%d,\"%s\",0", LinkID,topic);
	
   ret = atk_mw8266d_send_at_cmd(cmd, "OK", 1500);
	
   if (ret == ATK_MW8266D_EOK)
   {
       return ATK_MW8266D_EOK;
   }
   else
   {
       return ATK_MW8266D_ERROR;
   }
	
}
	

void recv_process(uint8_t *buf)
{
	printf("%s\r\n", buf);
}

void demo_receive_onenet_data(uint8_t is_atkcld ,uint8_t substatus)
{
    uint8_t *buf;
    
    if (is_atkcld == 1 && substatus ==0)
    {
        /* 接收来自ATK-MW8266D UART的一帧数据 */
        buf = atk_mw8266d_uart_rx_get_frame();
        if (buf != NULL)
        {
						recv_process(buf);
					  /* 重开开始接收来自ATK-MW8266D UART的数据 */
            atk_mw8266d_uart_rx_restart();	 
						
        }
    }
}



uint8_t demo_mqtt_userconf(void)
{
		uint8_t ret;
    char cmd[256];
    
    sprintf(cmd, "AT+MQTTUSERCFG=%d,%d,\"%s\",\"%s\",\"%s\",%d,%d,\"\"", 
						LinkID, scheme,client_id,username,password,cert_key_ID,CA_ID);
	
    ret = atk_mw8266d_send_at_cmd(cmd, "OK", 1500);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}

uint8_t demo_mqtt_connconf(void)
{
	uint8_t ret;
  char cmd[256];
    
    sprintf(cmd, "AT+MQTTCONNCFG=%d,%d,%d,\"\",\"\",%d,0", 
						LinkID, keepalive,disable_clean_session,lwt_qos);
	
    ret = atk_mw8266d_send_at_cmd(cmd, "OK", 1500);
	
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}

uint8_t atk_mw8266d_connect_onenet_server(char *server_ip, int server_port)
{
    uint8_t ret;
    char cmd[64];
    
    sprintf(cmd, "AT+MQTTCONN=0,\"%s\",%d,1", server_ip, server_port);
    printf("mqtt connect->cmd:%s\r\n",cmd);
    ret = atk_mw8266d_send_at_cmd(cmd, "OK", 3000);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}


uint8_t atk_mw8266d_disconnect_onenet_server(void)
{
    uint8_t ret;
    ret = atk_mw8266d_send_at_cmd("AT+MQTTCLEAN=0", "OK", 1000);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}

