#include "./BSP/DMA/dma.h"

ADC_HandleTypeDef ADC1_Handler;		//ADC���
DMA_HandleTypeDef hdma_adc1;			//DMA���

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
uint32_t  AD_Data[AD_CHN];                              //AD�ɼ��˲����ֵ
__IO uint16_t ADC_ConvertedValue[AD_Count][AD_CHN];//DMA����

void MY_ADC_Init(uint32_t ch1,uint32_t ch2)
{ 
	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
	
	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC����ʱ��
	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;			//��Ƶ����6ʱ��Ϊ72M/6=12MHz
	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					//����ADCʱ��
	
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=ADC_SCAN_ENABLE;                      //���ŵ�ɨ��ģʽ
    ADC1_Handler.Init.ContinuousConvMode=ENABLE;                //��ģת������������ת��ģʽ
    ADC1_Handler.Init.NbrOfConversion=2;                         //2��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
		ADC1_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT; 					//ADC�����Ҷ���
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
	
	HAL_ADCEx_Calibration_Start(&ADC1_Handler);					 //У׼ADC
	
	//ͨ������
	ADC_ChannelConfTypeDef ADC1_ChanConf = {0};
    
	//��һ��ͨ��
    ADC1_ChanConf.Channel=ch1;                                   //ͨ��
    ADC1_ChanConf.Rank=ADC_REGULAR_RANK_1;                      //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //����ʱ��               
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
	//�ڶ���ͨ��
		ADC1_ChanConf.Channel=ch2;                                   //ͨ��
    ADC1_ChanConf.Rank=ADC_REGULAR_RANK_2;                      //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //����ʱ��               
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
	//DMA Init
	__HAL_LINKDMA(&ADC1_Handler,DMA_Handle,hdma_adc1);
		hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;                //�����赽�ڴ�  
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;                    //�����ַ�Ĵ�������
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;                        //�ڴ��ַ�Ĵ�������
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;   //���ݿ��Ϊ16λ
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;      //���ݿ��Ϊ16λ
    hdma_adc1.Init.Mode = DMA_CIRCULAR;                             //ѭ������ģʽ
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;                    //DMAͨ�� xӵ�и����ȼ�
		HAL_DMA_Init(&hdma_adc1);
		

		HAL_DMA_Start(&hdma_adc1, (uint32_t)&ADC1_Handler.Instance->DR,(uint32_t)&ADC_ConvertedValue , AD_CHN * AD_Count);//���� DMA ����
    HAL_ADC_Start_DMA(&ADC1_Handler,(uint32_t *)&ADC_ConvertedValue, AD_CHN * AD_Count );  //�˺����Ŀ���ʱ�����Ӱ�����յĽ����������У������ȿ����������
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance==ADC1)
  {
		//ʹ��DMA�ж�
		HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
		
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOCʱ��
		__HAL_RCC_DMA1_CLK_ENABLE();			//����DMAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_2 | GPIO_PIN_1;            //PC0��PC1
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	}
}

//ȡƽ��ֵ
void ADC_Filter(void)
{
	uint8_t i, j;	
	for(i=0; i<AD_CHN; i++)/*ȡֵ���ȡƽ��*/
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
