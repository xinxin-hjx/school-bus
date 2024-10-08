#include "./BSP/ADC/mq135.h"
#include "./SYSTEM/delay/delay.h"
#include "math.h"

//ADC_HandleTypeDef ADC1_Handler;		//ADC句柄

////初始化ADC
////ch: ADC_channels 
////通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//void MY_ADC_Init(void)
//{ 
//	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
//	
//	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC外设时钟
//	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;			//分频因子6时钟为72M/6=12MHz
//	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					//设置ADC时钟
//	
//    ADC1_Handler.Instance=ADC1;
//    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
//    ADC1_Handler.Init.ScanConvMode=ADC_SCAN_DISABLE;                      //非扫描模式
//    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
//    ADC1_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
//    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
//    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
//    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
//    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
//	
//	HAL_ADCEx_Calibration_Start(&ADC1_Handler);					 //校准ADC
//}

////ADC底层驱动，引脚配置，时钟使能
////此函数会被HAL_ADC_Init()调用
////hadc:ADC句柄
//void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
//{
//    GPIO_InitTypeDef GPIO_Initure;
//    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
//    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
//	
//    GPIO_Initure.Pin=GPIO_PIN_0;            //PC0
//    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
//    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
//    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
//}

////获得ADC值
////ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
////返回值:转换结果
//uint16_t Get_Adc(uint32_t ch)   
//{
//    ADC_ChannelConfTypeDef ADC1_ChanConf;
//    
//    ADC1_ChanConf.Channel=ch;                                   //通道
//    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //采样时间               
//    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
//	
//    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
//	
//    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
// 
//	return (uint16_t)HAL_ADC_GetValue(&ADC1_Handler);	        	//返回最近一次ADC1规则组的转换结果
//}
////获取指定通道的转换值，取times次,然后平均 
////times:获取次数
////返回值:通道ch的times次转换结果平均值
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

////取times次,然后平均 
//float Get_MQ135Value(uint8_t times)
//{
//	float tmp,ppm;
//  int Val;
//	Val = Get_Adc_Average(ADC_CHANNEL_10,times);
//	tmp = (float)Val*5 / 4096;
//	ppm = pow((314.01*tmp/23.5-7.12*tmp),1/0.8394);
//	return ppm;
//} 

//返回值:通道ch1的times次转换结果平均值
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

//取times次,然后平均 
float Get_MQ135Value(uint8_t times)
{
	float tmp,ppm;
  int Val;
	Val = Get_Adc_Average(times);
	tmp = (float)Val*5 / 4096;
	ppm = pow((314.01*tmp/23.5-7.12*tmp),1/0.8394);
	return ppm;
}
