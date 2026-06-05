#ifndef DELAY_TIM2_H
#define DELAY_TIM2_H
#include "board.h"
void TIM2_Delay_Init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
#endif
