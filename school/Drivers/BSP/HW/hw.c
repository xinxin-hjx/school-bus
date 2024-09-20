#include "./BSP/HW/hw.h"
#include "./BSP/MP3/mp3.h"
void HW_Init(void)
{
	GPIO_InitTypeDef HW_GP;
	HW_GPIO_ENABLE();
	HW_GP.Mode =GPIO_MODE_IT_FALLING | GPIO_MODE_INPUT; 
	HW_GP.Pin = HW_PIN;
	HW_GP.Pull = GPIO_NOPULL;
	HW_GP.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HW_PORT,&HW_GP);
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,0);       
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
}
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(HW_PIN);
}
uint8_t Infrared_Flag = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == HW_PIN)
	{
		Infrared_Flag = 1;
	}
}

void HW_run(void)
{
	if(Infrared_Flag == 1)
	{
		LED1(0);
		BEEP(1);
		voice_select(2);
		delay_ms(100);
		Infrared_Flag = 0;
		
		
	} 
	if(Infrared_Flag == 0)
	{	
		if(HW_Read())
		{
			LED1(1);
			BEEP(0);
			 			
		}
	}
}



