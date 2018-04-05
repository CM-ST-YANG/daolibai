#include "pwm.h"

PWM_TypeDef PWM;
TIM_HandleTypeDef htim9;



void PWM_init(void)
{
	GPIO_InitTypeDef gpio_init;
	TIM_OC_InitTypeDef oc_init;

	__HAL_RCC_TIM9_CLK_ENABLE();
	
	gpio_init.Pin = GPIO_PIN_5 | GPIO_PIN_6;  //TIM9
	gpio_init.Mode	= GPIO_MODE_AF_PP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF3_TIM9;
	__HAL_RCC_GPIOE_CLK_ENABLE();
	HAL_GPIO_Init(GPIOE, &gpio_init);

	htim9.Instance					= TIM9;
	htim9.Init.Prescaler			= 24;
	htim9.Init.Period				= PWM_Period - 1;
	htim9.Init.CounterMode			= TIM_COUNTERMODE_UP;
	htim9.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	htim9.Init.RepetitionCounter	= 0;

	HAL_TIM_PWM_Init(&htim9); //定时器9 PWM输出初始化
	
	oc_init.OCMode			= TIM_OCMODE_PWM1;			//输出捕获方式设置
	oc_init.OCPolarity		= TIM_OCPOLARITY_HIGH;
	oc_init.OCNPolarity		= TIM_OCNPOLARITY_LOW;
	oc_init.OCFastMode		= TIM_OCFAST_DISABLE;
	oc_init.OCNIdleState	= TIM_OCNIDLESTATE_RESET;
	oc_init.OCIdleState		= TIM_OCIDLESTATE_RESET;
	oc_init.Pulse			= 0;
	
	TIM9->CCR1 = 5000;
	TIM9->CCR2 = 5000;
	
	HAL_TIM_PWM_ConfigChannel(&htim9, &oc_init, TIM_CHANNEL_1); //定时器9 PWM CH1-CH2 初始化
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);

	HAL_TIM_PWM_ConfigChannel(&htim9, &oc_init, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
	


}

//void set_pwm_val(char CH, uint32_t val)  //设置PWM输出值
//{
//	switch (CH)
//	{	
//	case 0:__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, (val > PWM_Period ? PWM_Period : val)); break;  //PE5
//	case 1:__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, (val > PWM_Period ? PWM_Period : val)); break;  //PE6
//	default:
//		break;
//	}
//}


