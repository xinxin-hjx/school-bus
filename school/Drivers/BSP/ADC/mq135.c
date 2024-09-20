#include "./BSP/ADC/mq135.h"
#include "./SYSTEM/delay/delay.h"
#include "math.h"

//ADC_HandleTypeDef ADC1_Handler;		//ADC���

////��ʼ��ADC
////ch: ADC_channels 
////ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//void MY_ADC_Init(void)
//{ 
//	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
//	
//	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC����ʱ��
//	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;			//��Ƶ����6ʱ��Ϊ72M/6=12MHz
//	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					//����ADCʱ��
//	
//    ADC1_Handler.Instance=ADC1;
//    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
//    ADC1_Handler.Init.ScanConvMode=ADC_SCAN_DISABLE;                      //��ɨ��ģʽ
//    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
//    ADC1_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
//    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
//    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
//    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
//    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
//	
//	HAL_ADCEx_Calibration_Start(&ADC1_Handler);					 //У׼ADC
//}

////ADC�ײ��������������ã�ʱ��ʹ��
////�˺����ᱻHAL_ADC_Init()����
////hadc:ADC���
//void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
//{
//    GPIO_InitTypeDef GPIO_Initure;
//    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
//    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOCʱ��
//	
//    GPIO_Initure.Pin=GPIO_PIN_0;            //PC0
//    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
//    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
//    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
//}

////���ADCֵ
////ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
////����ֵ:ת�����
//uint16_t Get_Adc(uint32_t ch)   
//{
//    ADC_ChannelConfTypeDef ADC1_ChanConf;
//    
//    ADC1_ChanConf.Channel=ch;                                   //ͨ��
//    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
//    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //����ʱ��               
//    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
//	
//    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
//	
//    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
// 
//	return (uint16_t)HAL_ADC_GetValue(&ADC1_Handler);	        	//�������һ��ADC1�������ת�����
//}
////��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
////times:��ȡ����
////����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
//uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
//{
//	uint32_t temp_val=0;
//	uint8_t t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 

////ȡtimes��,Ȼ��ƽ�� 
//float Get_MQ135Value(uint8_t times)
//{
//	float tmp,ppm;
//  int Val;
//	Val = Get_Adc_Average(ADC_CHANNEL_10,times);
//	tmp = (float)Val*5 / 4096;
//	ppm = pow((314.01*tmp/23.5-7.12*tmp),1/0.8394);
//	return ppm;
//} 

//����ֵ:ͨ��ch1��times��ת�����ƽ��ֵ
uint32_t Get_Adc_Average(uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD_Data[0];
		delay_ms(5);
	}
	return temp_val/times;
} 

//ȡtimes��,Ȼ��ƽ�� 
float Get_MQ135Value(uint8_t times)
{
	float tmp,ppm;
  int Val;
	Val = Get_Adc_Average(times);
	tmp = (float)Val*5 / 4096;
	ppm = pow((314.01*tmp/23.5-7.12*tmp),1/0.8394);
	return ppm;
}
