#include "./BSP/OLED/oled.h"
#include "string.h"
static uint8_t last_temp = 0;  
static uint8_t last_humi = 0;

/*引脚初始化*/
void OLED_I2C_Init(void)
{
	/*开启时钟*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/*GPIO配置*/
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

/*I2C开始*/
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/*I2C停止*/
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/*I2C发送一个字节*/
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/*OLED写命令*/
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/*OLED写数据*/
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/*OLED清屏*/
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
	*函数名    ：OLED_ShowChar
	*函数功能  ：显示一个16*16的字符
	*函数参数  ：uint8_t page,uint8_t list,uint8_t ch
	*函数返回值：无
	*函数描述  ：16*16  实际是 8*16   字高占用2页	
							 page:0~7
							 list:2~128
*/
void OLED_ShowChar(uint8_t page,uint8_t list,uint8_t ch)
{      	
	uint8_t i,j;
	uint8_t n;
	/*计算要显示的字符与空格的偏移值*/
	n = ch - ' ';         //
	
	/*显示*/
	for(i=0;i<2;i++)                           //页循环
	{
		OLED_SetCursor(page+i,list);            //+i人为换页
		for(j=0;j<8;j++)                       //列循环
		{
			OLED_WriteData(F16X16[n*16+j+8*i]);
		}
	}
}


/************************************************
*函数名    ：oled_dis_char
*函数功能  ：显示一个字符
*函数参数  ：uint8_t page,uint8_t list,uint8_t ch
*函数返回值：无
*函数描述  ：w:形式宽度（实际宽度在程序中处理）   32
            h:形式高度（/8）
						 page:0~7
						 list:2~128
*************************************************/
void oled_dis_char(uint8_t page,uint8_t list,uint8_t ch,uint8_t w,uint8_t h)
{
	uint8_t i,j;
	uint8_t n;
	/*计算要显示的字符与空格的偏移值*/
	n = ch - ' ';
	
	/*显示*/
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
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
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
*函数名    ：oled_dis_hz
*函数功能  ：显示一个的汉字
*函数参数  ：uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h
*函数返回值：无
*函数描述  ：
						 page:0~7
						 list:2~128
*************************************************/
void oled_dis_hz(uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h)
{
	uint8_t n=0;
	uint8_t i,j;
	/*计算要显示的数据与字库第一个字的偏移值*/
	while(table[2*n] != '\0')
	{
		if(*hz==table[2*n] && *(hz+1)==table[2*n+1])
		{
			break;
		}
		n++;
	}
	/*如果汉字库里面没有输入的汉字*/
	if(n == strlen((char *)table)/2)
	{
		return ;
	}
	
	/*显示*/
		w=h;
	//页循环
	for(i=0;i<h/8;i++)
	{
		//确认初始地址
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
*函数名    ：oled_dis_hz_str
*函数功能  ：显示一个的汉字串
*函数参数  ：uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h
*函数返回值：无
*函数描述  ：
						 page:0~7
						 list:2~128
*************************************************/
void oled_dis_hz_str(uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h)
{
	while(*hz!='\0')
	{
		oled_dis_hz(page,list,hz,w,h);
		//显示下一个汉字
		hz+=2;
		//屏幕
		list += w;
		
		if(list>128-w)
		{
			page += h/8;
			list = 2;
		}
	}
}

/************************************************
*函数名    ：OLED_Show
*函数功能  ：显示一串汉字和字符
*函数参数  ：uint8_t page,uint8_t list,uint8_t *hz,uint8_t w,uint8_t h
*函数返回值：无
*函数描述  ：
						 page:0~7
						 list:2~128
*************************************************/
void OLED_Show(uint8_t page,uint8_t list,uint8_t *str,uint8_t w,uint8_t h)
{
	while(*str!='\0')                         //遍历每个数组中的每个元素
	{
		if(*str>=32 && *str<=127)               //判断是否为字符 
		{
			oled_dis_char(page,list,*str,w,h);    //调用显示一个字符函数
			str++;                                //换数组下一个元素
			list+=w/2;                            //屏幕位置换下一个位置k
			if(list>128-w/2)                      //判断是否需要换页
			{
				page+=h/8;
				list=2;
			}
		}
		else                                    //不是字符就是汉字
		{
			oled_dis_hz(page,list,str,w,h);      //调用显示一个汉字函数
			str+=2;                              //换数组下一个元素（汉字大小占两个位）
			list+=h;                             //屏幕位置换下一个位置（汉字的w和h是一样的）
			if(list>128-w)                       //判断是否需要换页 
			{
				page+=h/8;
				list=2;
			}
		}
	}
}

/************************************************
*函数名    ：OLED_ShowPic
*函数功能  ：显示一个图形
*函数参数  ：uint8_t page,uint8_t list,const uint8_t *pic,uint8_t w,uint8_t h
*函数返回值：无
*函数描述  ：
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
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
void update_display() {  
    uint8_t temp, humi;  
  
    // 读取DHT11数据  
    dht11_read_data(&temp, &humi);  
    // 检查新数据是否与旧数据不同  
    if (temp != last_temp || humi != last_humi) {  
        // 数据已更改，更新显示  
        OLED_Clear();  
  
        uint8_t tem[] = "温度";  
        OLED_Show(0, 0, tem, 16, 16);  
  
        uint8_t hum[] = "湿度";  
        OLED_Show(50, 0, hum, 16, 16);  
  
        char tempStr[10], humiStr[10];  
        sprintf(tempStr, "%d", temp);  
        sprintf(humiStr, "%d", humi);  
  
        OLED_ShowString(0, 33, tempStr, 16, 16);  
        OLED_ShowString(50, 33, humiStr, 16, 16);  
  
        // 更新最后一次读取的数据  
        last_temp = temp;  
        last_humi = humi;  
    }  
  
    // 延时一段时间再次检查（或根据你的需求进行调整）  
    delay_ms(1000);  
} 
