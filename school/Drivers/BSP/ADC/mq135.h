#ifndef MQ135_H_
#define MQ135_H_

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/DMA/dma.h"

uint32_t Get_Adc_Average(uint8_t times);//�õ�ĳ��ͨ����������������ƽ��ֵ
float Get_MQ135Value(uint8_t times);

#endif
