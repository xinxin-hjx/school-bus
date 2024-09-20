#include "./BSP/OLED/oled.h"
#include "string.h"
static uint8_t last_temp = 0;  
static uint8_t last_humi = 0;

/*���ų�ʼ��*/
void OLED_I2C_Init(void)
{
	/*����ʱ��*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/*GPIO����*/
	GPIO_InitTypeDef  GPIO_Initure = {0};
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_Initure.Pin = GPIO_PIN_7;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/*I2C��ʼ*/
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/*I2Cֹͣ*/
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/*I2C����һ���ֽ�*/
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//�����һ��ʱ�ӣ�������Ӧ���ź�
	OLED_W_SCL(0);
}

/*OLEDд����*/
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x00);		//д����
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/*OLEDд����*/
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x40);		//д����
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//����Yλ��
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�õ�4λ
}

/*OLED����*/
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}


/**
	*������    ��OLED_ShowChar
	*��������  ����ʾһ��16*16���ַ�
	*��������  ��uint8_t page,uint8_t list,uint8_t ch
	*��������ֵ����
	*��������  ��16*16  ʵ���� 8*16   �ָ�ռ��2ҳ	
							 page:0~7
							 list:2~128
*/
void OLED_ShowChar(uint8_t page,uint8_t list,uint8_t ch)
{      	
	uint8_t i,j;
	uint8_t n;
	/*����Ҫ��ʾ���ַ���ո��ƫ��ֵ*/
	n = ch - ' ';         //
	
	/*��ʾ*/
	for(i=0;i<2;i++)                           //ҳѭ��
	{
		OLED_SetCursor(page+i,list);            //+i��Ϊ��ҳ
		for(j=0;j<8;j++)                       //��ѭ��
		{
			OLED_WriteData(F16X16[n*16+j+8*i]);
		}
	}
}


/************************************************
*������    ��oled_dis_char
*��������  ����ʾһ���ַ�
*��������  ��uint8_t page,uint8_t list,uint8_t ch
*��������ֵ����
*��������  ��w:��ʽ��ȣ�ʵ�ʿ���ڳ����д���   32
            h:��ʽ�߶ȣ�/8��
						 page:0~7
						 list:2~128
*************************************************/
void oled_dis_char(uint8_t page,uint8_t list,uint8_t ch,uint8_t w,uint8_t h)
{
	uint8_t i,j;
	uint8_t n;
	/*����Ҫ��ʾ���ַ���ո��ƫ��ֵ*/
	n = ch - ' ';
	
	/*��ʾ*/
	for(i=0;i<h/8;i++)
	{
		OLED_SetCursor(page+i,list);
		for(j=0;j<w/2;j++)
		{
			if(w==16 && h==16)
			{
				OLED_WriteData(F16X16[n*16+j+8*i]);
			}
			else if(w==32 && h==24)
			{
				OLED_WriteData(F32X24[n*48+j+16*i]);
			}
			
		}
	}
	
}

/**
  * @brief  OLED��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowString(uint8_t page, uint8_t list, const char* str,uint8_t w,uint8_t h)
{
	while(*str!= '\0')
	{
		oled_dis_char(page,list,*str,w,h);
		str++;
		list+=w/2;
		if(list>128-w/2)
		{
			page+=h/8;
			list=2;
		}
	}
}

/************************************************
*������    ��oled_dis_hz
*��������  ����ʾһ���ĺ���
*��������  ��uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h
*��������ֵ����
*��������  ��
						 page:0~7
						 list:2~128
*************************************************/
void oled_dis_hz(uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h)
{
	uint8_t n=0;
	uint8_t i,j;
	/*����Ҫ��ʾ���������ֿ��һ���ֵ�ƫ��ֵ*/
	while(table[2*n] != '\0')
	{
		if(*hz==table[2*n] && *(hz+1)==table[2*n+1])
		{
			break;
		}
		n++;
	}
	/*������ֿ�����û������ĺ���*/
	if(n == strlen((char *)table)/2)
	{
		return ;
	}
	
	/*��ʾ*/
		w=h;
	//ҳѭ��
	for(i=0;i<h/8;i++)
	{
		//ȷ�ϳ�ʼ��ַ
		OLED_SetCursor(page+i,list);
	
		for(j=0;j<w;j++)
		{
			if(h==16 && w==16)
			{
				OLED_WriteData(hz16[n*32+j+16*i]);
			}
			else if(h==24 && w==24)
			{
				OLED_WriteData(hz24[n*72+j+24*i]);
			}
		}
		
	}
}

/************************************************
*������    ��oled_dis_hz_str
*��������  ����ʾһ���ĺ��ִ�
*��������  ��uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h
*��������ֵ����
*��������  ��
						 page:0~7
						 list:2~128
*************************************************/
void oled_dis_hz_str(uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h)
{
	while(*hz!='\0')
	{
		oled_dis_hz(page,list,hz,w,h);
		//��ʾ��һ������
		hz+=2;
		//��Ļ
		list += w;
		
		if(list>128-w)
		{
			page += h/8;
			list = 2;
		}
	}
}

/************************************************
*������    ��OLED_Show
*��������  ����ʾһ�����ֺ��ַ�
*��������  ��uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h
*��������ֵ����
*��������  ��
						 page:0~7
						 list:2~128
*************************************************/
void OLED_Show(uint8_t page,uint8_t list,uint8_t *str,uint8_t w,uint8_t h)
{
	while(*str!='\0')                         //����ÿ�������е�ÿ��Ԫ��
	{
		if(*str>=32 && *str<=127)               //�ж��Ƿ�Ϊ�ַ� 
		{
			oled_dis_char(page,list,*str,w,h);    //������ʾһ���ַ�����
			str++;                                //��������һ��Ԫ��
			list+=w/2;                            //��Ļλ�û���һ��λ��k
			if(list>128-w/2)                      //�ж��Ƿ���Ҫ��ҳ
			{
				page+=h/8;
				list=2;
			}
		}
		else                                    //�����ַ����Ǻ���
		{
			oled_dis_hz(page,list,str,w,h);      //������ʾһ�����ֺ���
			str+=2;                              //��������һ��Ԫ�أ����ִ�Сռ����λ��
			list+=h;                             //��Ļλ�û���һ��λ�ã����ֵ�w��h��һ���ģ�
			if(list>128-w)                       //�ж��Ƿ���Ҫ��ҳ 
			{
				page+=h/8;
				list=2;
			}
		}
	}
}

/************************************************
*������    ��OLED_ShowPic
*��������  ����ʾһ��ͼ��
*��������  ��uint8_t page,uint8_t list,const uint8_t *pic,uint8_t w,uint8_t h
*��������ֵ����
*��������  ��
						 page:0~7
						 list:2~128
*************************************************/
void OLED_ShowPic(uint8_t page,uint8_t list,const uint8_t *pic,uint8_t w,uint8_t h)
{
	uint8_t page_n;
	uint8_t i,j;
	if(h%8==0)
	{
		page_n=h/8;
	}
	else
	{
		page_n=h/8+1;
	}
	for(i=0;i<page_n;i++)
	{
		OLED_SetCursor(page+i,list);
		for(j=0;j<w;j++)
		{
			OLED_WriteData(pic[w*i+j]);
		}
	}
}



/**
  * @brief  OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//�˿ڳ�ʼ��
	
	OLED_WriteCommand(0xAE);	//�ر���ʾ
	
	OLED_WriteCommand(0xD5);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//���ö�·������
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//������ʾƫ��
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//������ʾ��ʼ��
	
	OLED_WriteCommand(0xA1);	//�������ҷ���0xA1���� 0xA0���ҷ���
	
	OLED_WriteCommand(0xC8);	//�������·���0xC8���� 0xC0���·���

	OLED_WriteCommand(0xDA);	//����COM����Ӳ������
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//���öԱȶȿ���
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//����Ԥ�������
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//����VCOMHȡ��ѡ�񼶱�
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//����������ʾ��/�ر�

	OLED_WriteCommand(0xA6);	//��������/��ת��ʾ

	OLED_WriteCommand(0x8D);	//���ó���
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//������ʾ
		
	OLED_Clear();				//OLED����
}
void update_display() {  
    uint8_t temp, humi;  
  
    // ��ȡDHT11����  
    dht11_read_data(&temp, &humi);  
    // ����������Ƿ�������ݲ�ͬ  
    if (temp != last_temp || humi != last_humi) {  
        // �����Ѹ��ģ�������ʾ  
        OLED_Clear();  
  
        uint8_t tem[] = "�¶�";  
        OLED_Show(0, 0, tem, 16, 16);  
  
        uint8_t hum[] = "ʪ��";  
        OLED_Show(50, 0, hum, 16, 16);  
  
        char tempStr[10], humiStr[10];  
        sprintf(tempStr, "%d", temp);  
        sprintf(humiStr, "%d", humi);  
  
        OLED_ShowString(0, 33, tempStr, 16, 16);  
        OLED_ShowString(50, 33, humiStr, 16, 16);  
  
        // �������һ�ζ�ȡ������  
        last_temp = temp;  
        last_humi = humi;  
    }  
  
    // ��ʱһ��ʱ���ٴμ�飨��������������е�����  
    delay_ms(1000);  
} 
