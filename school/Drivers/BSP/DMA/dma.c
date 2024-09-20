#include "./BSP/DMA/dma.h"

ADC_HandleTypeDef ADC1_Handler;		//ADC句柄
DMA_HandleTypeDef hdma_adc1;			//DMA句柄

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
uint32_t  AD_Data[AD_CHN];                              //AD采集滤波后的值
__IO uint16_t ADC_ConvertedValue[AD_Count][AD_CHN];//DMA缓存

void MY_ADC_Init(uint32_t ch1,uint32_t ch2)
{ 
	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
	
	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC外设时钟
	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;			//分频因子6时钟为72M/6=12MHz
	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					//设置ADC时钟
	
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=ADC_SCAN_ENABLE;                      //多信道扫描模式
    ADC1_Handler.Init.ContinuousConvMode=ENABLE;                //数模转换工作在连续转换模式
    ADC1_Handler.Init.NbrOfConversion=2;                         //2个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
		ADC1_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT; 					//ADC数据右对齐
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
	
	HAL_ADCEx_Calibration_Start(&ADC1_Handler);					 //校准ADC
	
	//通道配置
	ADC_ChannelConfTypeDef ADC1_ChanConf = {0};
    
	//第一个通道
    ADC1_ChanConf.Channel=ch1;                                   //通道
    ADC1_ChanConf.Rank=ADC_REGULAR_RANK_1;                      //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //采样时间               
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
	
	//第二个通道
		ADC1_ChanConf.Channel=ch2;                                   //通道
    ADC1_ChanConf.Rank=ADC_REGULAR_RANK_2;                      //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //采样时间               
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
	
	//DMA Init
	__HAL_LINKDMA(&ADC1_Handler,DMA_Handle,hdma_adc1);
		hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;                //从外设到内存  
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;                    //外设地址寄存器不变
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;                        //内存地址寄存器递增
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;   //数据宽度为16位
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;      //数据宽度为16位
    hdma_adc1.Init.Mode = DMA_CIRCULAR;                             //循环工作模式
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;                    //DMA通道 x拥有高优先级
		HAL_DMA_Init(&hdma_adc1);
		

		HAL_DMA_Start(&hdma_adc1, (uint32_t)&ADC1_Handler.Instance->DR,(uint32_t)&ADC_ConvertedValue , AD_CHN * AD_Count);//开启 DMA 传输
    HAL_ADC_Start_DMA(&ADC1_Handler,(uint32_t *)&ADC_ConvertedValue, AD_CHN * AD_Count );  //此函数的开启时间可能影响最终的结果，如果不行，就率先开启这个函数
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance==ADC1)
  {
		//使能DMA中断
		HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
		
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
		__HAL_RCC_DMA1_CLK_ENABLE();			//开启DMA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_2 | GPIO_PIN_1;            //PC0、PC1
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	}
}

//取平均值
void ADC_Filter(void)
{
	uint8_t i, j;	
	for(i=0; i<AD_CHN; i++)/*取值求和取平均*/
	{
		 AD_Data[i] = 0;
		 for(j=0; j<AD_Count; j++)
		 {
			 AD_Data[i] += ADC_ConvertedValue[j][i];	
			 //USART1_printf(" %4d   j = %d   i = %d" ,ADC_ConvertedValue[j][i],j,i);
		 }
		 //USART1_printf("\n");
		 AD_Data[i] = AD_Data[i] / AD_Count;
	}
}
