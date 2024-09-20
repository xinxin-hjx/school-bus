#ifndef __HW_H
#define __HW_H
#include "./SYSTEM/sys/sys.h"
#include "./BSP/MQTT/mqtt.h"
#include "./BSP/LED/led.h"
#include "./BSP/BEEP/beep.h"

#define HW_PORT GPIOB
#define HW_PIN  GPIO_PIN_9
#define HW_GPIO_ENABLE() do{__HAL_RCC_GPIOB_CLK_ENABLE();} while(0)
#define HW_Read() HAL_GPIO_ReadPin(HW_PORT,HW_PIN)
void HW_Init(void);
void HW_run(void);
extern uint8_t Infrared_Flag;

#endif


