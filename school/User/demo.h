#ifndef __DEMO_H
#define __DEMO_H


#include "./BSP/MQTT/mqtt.h"
#include "./BSP/HW/hw.h"
#include "./BSP/DMA/dma.h"
#include "./BSP/ADC/mq135.h"
#include "./BSP/AIRC/airc.h"
#define AT_XC_BUF "{\\\"id\\\":123\\\,\\\"dp\\\":{\\\"bus\\\":[{\\\"v\\\": %d}]}}"

void demo_run(void);

#endif
