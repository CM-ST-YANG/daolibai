#include "OLEDUPT.h"
#include "stm32f4xx_hal.h"
#include <delay.h>
#include <IIC.h>


extern int32_t SPEED[2];
char a[] = "i love you";
TIM_HandleTypeDef htim3;

void tim3_init(void)
{
	uint16_t prescalervalue = 0;
	uint32_t tmpvalue = 0;

	  /* TIM4 clock enable */
	__HAL_RCC_TIM3_CLK_ENABLE();

	  /* Enable the TIM4 global Interrupt */
	HAL_NVIC_SetPriority(TIM3_IRQn, 6, 0);  
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
  

  
	/* Compute the prescaler value */
	tmpvalue = HAL_RCC_GetPCLK1Freq();
	prescalervalue = (uint16_t)((tmpvalue * 2) / 1000000) - 1;
  
	/* Time base configuration */
	htim3.Instance = TIM3;
	htim3.Init.Period = 5000;
	htim3.Init.Prescaler = prescalervalue;
	htim3.Init.ClockDivision = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;

	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		/* Initialization Error */
		//Error_Handler();
	}

	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
	{
		/* Start Error */
	   // Error_Handler();
	}
}

void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim3);
}


uint32_t tim4_cnt;

void  TIM3_Handler(void)
{	
	SPEED[0] = TIM4->CNT - 1024;
	TIM4->CNT = 1024;
}
