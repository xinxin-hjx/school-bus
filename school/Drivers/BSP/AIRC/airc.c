#include "./BSP/AIRC/airc.h"

void airc_init(void)
{
	GPIO_InitTypeDef GPIO_AIRC;
	airc_CLK_ENABLE();
	GPIO_AIRC.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_AIRC.Pin = airc_PIN;
	GPIO_AIRC.Pull = GPIO_PULLUP;
	GPIO_AIRC.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(airc_PORT, &GPIO_AIRC);
	airc(0);

}
//void FS_test(void)
//{	
//	uint8_t tempa,humia;
//	dht11_read_data(&tempa,&humia);
//	if(tempa >=30)
//	{
//		airc(1);	
//	}
//	else if(tempa < 30)
//	{
//		airc(0);
//	}

//}
