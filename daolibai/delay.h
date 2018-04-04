#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"
#define CPU_FREQUENCY_MHZ    168
	void delay_us(uint16_t time);
#ifdef __cplusplus
}
#endif

#endif
