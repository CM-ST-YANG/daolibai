#include <IIC.h>
#include <delay.h>
GPIO_InitTypeDef iic1;
GPIO_InitTypeDef RST;

void IIC_INIT()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	iic1.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	iic1.Mode = GPIO_MODE_OUTPUT_OD;
	iic1.Pull = GPIO_PULLUP;
	iic1.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &iic1); 
	IIC_SDA(HIGH);
	IIC_SCL(HIGH);
}

void RST_INIT()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	iic1.Pin = GPIO_PIN_5;
	iic1.Mode = GPIO_MODE_OUTPUT_PP;
	iic1.Pull = GPIO_NOPULL;
	iic1.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &iic1); 
}

void IIC_START()
{
	SDA1_OUT();
	IIC_SDA(HIGH);
	IIC_SCL(HIGH);
	delay_us(4);
	IIC_SDA(LOW);
	delay_us(4);
	IIC_SCL(LOW);	
}

void IIC_STOP()
{
	SDA1_OUT();
	IIC_SDA(LOW);
	IIC_SCL(HIGH);
	delay_us(4);
	IIC_SDA(HIGH);
	delay_us(4);
}

uint8_t IIC_WAIT_ACK()
{
	uint8_t AckTime;
	SDA1_IN();
	IIC_SDA(HIGH);
	delay_us(1);
	IIC_SCL(HIGH);
	delay_us(1);
	while (READ_SDA)
	{
		AckTime++;
		if (AckTime > 250)
		{
			IIC_STOP();
			return 1;
		}
	}
	IIC_SCL(LOW);
	return 0;
}

void IIC_ACK()
{
	SDA1_OUT();
	IIC_SDA(LOW);
	IIC_SCL(LOW);
	delay_us(2);
	IIC_SCL(HIGH);
	delay_us(2);
	IIC_SCL(LOW);
}

void IIC_NACK()
{
	SDA1_OUT();
	IIC_SCL(LOW);
	IIC_SDA(HIGH);
	delay_us(2);
	IIC_SCL(HIGH);
	delay_us(2);
	IIC_SCL(LOW);
}

void IIC_SEND_BATE(uint8_t txd)
{
	uint8_t t;
	SDA1_OUT();
	IIC_SCL(LOW);
	for (t = 0;t < 8;t++)
	{
		IIC_SDA((txd & 0x80) >> 7); 
		txd <<= 1; 
		delay_us(2); 
		IIC_SCL(HIGH);
		delay_us(2);  
		IIC_SCL(LOW);  
		delay_us(2);
	}
}

uint8_t IIC_READ_BYTE(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA1_IN();
	for (i = 0; i < 8; i++) 
	{
		IIC_SCL(LOW);  
		delay_us(2);
		IIC_SCL(HIGH);
		receive <<= 1;  
		if (READ_SDA) receive++; 
		delay_us(1); 
	}
	if (!ack) IIC_NACK(); 
	else IIC_ACK();
	return receive;
}

void WriteCmd(uint8_t command)
{
	IIC_START();
	IIC_SEND_BATE(0x78);//OLED地址
	IIC_WAIT_ACK();
	IIC_SEND_BATE(0x00);//寄存器地址
	IIC_WAIT_ACK();
	IIC_SEND_BATE(command);
	IIC_WAIT_ACK();
	IIC_STOP();
}

void WriteDat(uint8_t data)
{
	IIC_START();
	IIC_SEND_BATE(0x78);//OLED地址
	IIC_WAIT_ACK();
	IIC_SEND_BATE(0x40);//寄存器地址
	IIC_WAIT_ACK();
	IIC_SEND_BATE(data);
	IIC_WAIT_ACK();
	IIC_STOP();
}

void OLED_Init(void)
{
	HAL_Delay(100); //这里的延时很重要

	WriteCmd(0xAE); //display off
	WriteCmd(0x20); //Set Memory Addressing Mode    
	WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8); //Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0 + y);
	WriteCmd(((x & 0xf0) >> 4) | 0x10);
	WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m, n;
	for (m = 0;m < 8;m++)
	{
		WriteCmd(0xb0 + m);       //page0-page1
		WriteCmd(0x00);     //low column start address
		WriteCmd(0x10);     //high column start address
		for (n = 0;n < 128;n++)
		{
			WriteDat(fill_Data);
		}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

void OLED_ShowStr(unsigned char x, unsigned char y,char ch[], unsigned char TextSize)
{
	unsigned char c = 0, i = 0, j = 0;
	switch (TextSize)
	{
	case 1:
		{
			while (ch[j] != '\0')
			{
				c = ch[j] - 32;
				if (x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x, y);
				for (i = 0;i < 6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
	case 2:
		{
			while (ch[j] != '\0')
			{
				c = ch[j] - 32;
				if (x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x, y);
				for (i = 0;i < 8;i++)
					WriteDat(F8X16[c * 16 + i]);
				OLED_SetPos(x, y + 1);
				for (i = 0;i < 8;i++)
					WriteDat(F8X16[c * 16 + i + 8]);
				x += 8;
				j++;
			}
		}break;
	}
}

void IIC_RST()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

//void IIC_WriteByte(uint8_t data)
//{
//	uint8_t i;
//	SDA1_OUT();
//	for (i = 0;i < 8;i++)
//	{
//		IIC_SCL(LOW);
//		delay_us(2);
//		if (data & 0x80)     //MSB,从高位开始一位一位传输
//			IIC_SDA(HIGH);
//		else
//			IIC_SDA(LOW);
//		IIC_SCL(HIGH);
//		delay_us(2);
//		IIC_SCL(LOW);
//		data <<= 1;
//
//	}
//}