#include <delay.h>
//static __IO uint32_t TimingDelay;

void delay_us(uint16_t time)
{    
	uint16_t i = 0;  
	while (time--)
	{
		i = 10;  //自己定义
		while (i--)	;    
	}
}
//	int old_val, new_val, val;
//
//	if (nTime > 900)
//	{
//		for (old_val = 0; old_val < nTime / 900; old_val++)
//		{
//			delay_us(900);
//		}
//		nTime = nTime % 900;
//	}
//
//	old_val = SysTick->VAL;
//	new_val = old_val - CPU_FREQUENCY_MHZ*nTime;
//	if (new_val >= 0)
//	{
//		do
//		{
//			val = SysTick->VAL;
//		} while ((val < old_val)&&(val >= new_val));
//	}
//	else
//	{
//		new_val += CPU_FREQUENCY_MHZ * 1000;
//		do
//		{
//			val = SysTick->VAL;
//		} while ((val <= old_val) || (val > new_val));
//
//	}
