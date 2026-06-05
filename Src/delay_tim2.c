#include "delay_tim2.h"

void TIM2_Delay_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->CR1 = 0U;
    TIM2->PSC = 16U - 1U;
    TIM2->ARR = 0xFFFFFFFFU;
    TIM2->CNT = 0U;

    TIM2->EGR |= TIM_EGR_UG;
    TIM2->SR = 0U;

    TIM2->CR1 |= TIM_CR1_CEN;
}

void delay_us(uint32_t us)
{
    TIM2->CNT = 0U;
    while (TIM2->CNT < us)
    {
    }
}

void delay_ms(uint32_t ms)
{
    while (ms--)
    {
        delay_us(1000U);
    }
}
