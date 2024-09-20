#ifndef _MQTT_H_
#define _MQTT_H_

#include "./SYSTEM/delay/delay.h"
#include "./BSP/ATK_MW8266D/atk_mw8266d.h"
#include "./BSP/ATK_MW8266D/atk_mw8266d_uart.h"
#include <string.h>
#include <stdio.h>
/*
	AT+MQTTUSERCFG=<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">
*/

/*OneNet平台：https://open.iot.10086.cn/console/device/manage/devs*/
#define LinkID 0
#define scheme 1
#define client_id "wsdsc"							//设备名称/ID
#define username	"45A9GPiOin"				//用户名为PID

/*
	使用token工具生成：https://open.iot.10086.cn/doc/v5/fuse/detail/1486
	version->默认2018-10-31
	res->设备级格式为：products/{产品id}/devices/{设备名称}
	et->比当前时间戳要大，尽量一年最好，使用链接生成时间戳：https://tool.lu/timestamp/
	key->设备密钥
	
*/
#define password  "version=2018-10-31&res=products%2F45A9GPiOin%2Fdevices%2Fwsdsc&et=1792535304&method=md5&sign=6%2F01c7lSl%2FMRgCZdBEi2UA%3D%3D"
#define cert_key_ID (0)
#define CA_ID				(0)


//AT+MQTTCONNCFG=<LinkID>,<keepalive>,<disable_clean_session>,<"lwt_topic">,<"lwt_msg">,<lwt_qos>,<lwt_retain>
#define keepalive 60 							//MQTT ping 超时时间，单位：秒。范围：[0,7200]。默认值：0，会被强制改为 120 秒。
#define disable_clean_session 0		//0: 使能清理会话 1: 禁用清理会话
#define lwt_qos 0									//遗嘱 QoS，参数可选 0、1、2，默认值：0。

#define PubTopic	"$sys/45A9GPiOin/wsdsc/dp/post/json"
#define PubResultTopic	"$sys/45A9GPiOin/wsdsc/dp/post/json"
#define PubQos		(0)


#define MQTT_MSG_MAX_SIZE 256
#define MQTT_MSG_HIG_SIZE 128
#define MQTT_MSG_MID_SIZE 64
#define MQTT_MSG_LOW_SIZE 32


uint8_t demo_mqtt_userconf(void);																						/* ATK-MW8266D配置MQTT用户信息 */
uint8_t demo_mqtt_connconf(void);																						/* ATK-MW8266D配置MQTT连接信息 */			
uint8_t atk_mw8266d_connect_onenet_server(char *server_ip, int server_port); /* ATK-MW8266D连接OneNet物联网平台 */
uint8_t atk_mw8266d_disconnect_onenet_server(void);													 /* ATK-MW8266D断开OneNet物联网平台 */

uint8_t demo_msg_pub(uint8_t pub_topic[MQTT_MSG_MID_SIZE] ,char *buf ,uint8_t qos);
void demo_receive_onenet_data(uint8_t is_atkcld ,uint8_t substatus);
uint8_t demo_topic_sub(char *topic);
#endif

