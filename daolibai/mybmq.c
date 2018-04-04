#include "mybmq.h"
#include "stm32f4xx_hal.h"

uint32_t A, B;
uint32_t test1=2,test2=0;
int value[] = { 0,1,3,2 };
int32_t SPEED[2];
GPIO_InitTypeDef gpio;
TIM_HandleTypeDef htim4;
TIM_Encoder_InitTypeDef tim4;
//TIM_OC_InitTypeDef tim4_oc;

void tim4_init(void)
{
	htim4.Instance = TIM4;
	htim4.Init.Period = 65535;
	htim4.Init.Prescaler = 0;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	tim4.EncoderMode = TIM_ENCODERMODE_TI12;
	tim4.IC1Filter = 0;
	tim4.IC1Polarity = TIM_ICPOLARITY_RISING;
	tim4.IC1Prescaler = TIM_ICPSC_DIV1;
	tim4.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	
	tim4.IC2Filter = 0;
	tim4.IC2Polarity = TIM_ICPOLARITY_RISING;
	tim4.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	tim4.IC2Prescaler = TIM_ICPSC_DIV1;
	HAL_TIM_Encoder_Init(&htim4, &tim4);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);
	TIM4->CNT = 1024;
	__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE); 
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{   
	GPIO_InitTypeDef GPIO_Initure;
	if (htim->Instance == TIM4)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE(); 
		__HAL_RCC_TIM4_CLK_ENABLE();           
		GPIO_Initure.Pin = GPIO_PIN_12 | GPIO_PIN_13;            
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;      
		GPIO_Initure.Pull = GPIO_PULLUP;          
		GPIO_Initure.Alternate = GPIO_AF2_TIM4;  
		HAL_GPIO_Init(GPIOD, &GPIO_Initure);
		
		TIM4->CNT = 1024;
		
		
//		__HAL_RCC_TIM4_CLK_ENABLE();
	}

}

void TIM4_Handler()
{
	
}


