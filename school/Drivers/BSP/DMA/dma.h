#ifndef _DMA_H
#define _DMA_H

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"

#define AD_CHN      1                  //Ҫʹ�õ�ADCͨ����
#define AD_Count   8			             //AD�˲���������

extern ADC_HandleTypeDef ADC1_Handler;	
extern uint32_t  AD_Data[AD_CHN];                              //AD�ɼ��˲����ֵ

void MY_ADC_Init(uint32_t ch1,uint32_t ch2);
void ADC_Filter(void);

#endif 
