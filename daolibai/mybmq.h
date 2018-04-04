#ifndef __MYBMQ_H__
#define __MYBMQ_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"
	void tim4_init(void);
//	void TIM4_Handler(void);
	int search(uint32_t x);
	void TIM4_Handler();
#ifdef __cplusplus
}
#endif

#endif

