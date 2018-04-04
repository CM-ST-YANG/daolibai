#include <bianmaqi.h>
TIM_HandleTypeDef htim3;
TIM_IC_InitTypeDef BMQ;
TIM_Encoder_InitTypeDef EN_BMQ;
void BMQ_INIT()
{	

	
	__HAL_RCC_TIM3_CLK_ENABLE();
//	HAL_TIM_Base_DeInit(&htim3);
//	HAL_TIM_Base_Start(&htim3);
	htim3.Instance					= TIM3;
	htim3.Init.Prescaler			= BQM_TIM_Prescaler;
	htim3.Init.Period				= BQM_TIM_Period - 1;
	htim3.Init.CounterMode			= TIM_COUNTERMODE_UP;
	htim3.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	htim3.Init.RepetitionCounter	= 0;
//	HAL_TIM_Base_Init(&htim3);
	EN_BMQ.EncoderMode = TIM_ENCODERMODE_TI2;
	EN_BMQ.IC1Polarity = TIM_ICPOLARITY_RISING;
	EN_BMQ.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	EN_BMQ.IC1Prescaler = TIM_ICPSC_DIV1;
	EN_BMQ.IC1Filter = 0;
	
	EN_BMQ.IC2Polarity = TIM_ICPOLARITY_RISING;
	EN_BMQ.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	EN_BMQ.IC2Prescaler = TIM_ICPSC_DIV1;
	EN_BMQ.IC2Filter = 0;
	
	HAL_TIM_Encoder_Init(&htim3, &EN_BMQ);
//	HAL_TIM_IC_Init(&htim3);
//	BMQ.ICFilter = 6;
	
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim_base)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if (htim_base->Instance == TIM3)
	{
//		ENCODER_TIM_RCC_CLK_ENABLE();
//		ENCODER_TIM_GPIO_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM3_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
//		bmq.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//    
//		GPIO_InitStruct.Pin = ENCODER_TIM_CH1_PIN;
//		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//		GPIO_InitStruct.Pull = GPIO_PULLUP;
//		HAL_GPIO_Init(ENCODER_TIM_CH1_GPIO, &GPIO_InitStruct);
//    
//		GPIO_InitStruct.Pin = ENCODER_TIM_CH2_PIN;
//		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//		GPIO_InitStruct.Pull = GPIO_PULLUP;
//		HAL_GPIO_Init(ENCODER_TIM_CH2_GPIO, &GPIO_InitStruct);
	}
}
//	
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	if (htim_base->Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_DISABLE();
    
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6 | GPIO_PIN_7);
	}
} 