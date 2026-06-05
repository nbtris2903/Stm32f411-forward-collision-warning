#include "hcsr04_reg.h"
#include "delay_tim2.h"
#define HCSR04_TIMEOUT_US    30000U
#define HCSR04_MIN_CM        2U
#define HCSR04_MAX_CM        400U
uint32_t HCSR04_Read_Reg(void)
{
   uint32_t time_us;
   uint32_t distance_cm;
   PIN_CLR(GPIOA, TRIG_PIN);
   delay_us(2U);
   PIN_SET(GPIOA, TRIG_PIN);
   delay_us(10U);
   PIN_CLR(GPIOA, TRIG_PIN);
   TIM2->CNT = 0U;
   while ((GPIOA->IDR & (1U << ECHO_PIN)) == 0U)
   {
       if (TIM2->CNT > HCSR04_TIMEOUT_US)
       {
           return HCSR04_MAX_CM;
       }
   }
   TIM2->CNT = 0U;
   while ((GPIOA->IDR & (1U << ECHO_PIN)) != 0U)
   {
       if (TIM2->CNT > HCSR04_TIMEOUT_US)
       {
           return HCSR04_MAX_CM;
       }
   }
   time_us = TIM2->CNT;
   distance_cm = time_us / 58U;
   if (distance_cm < HCSR04_MIN_CM)
   {
       return HCSR04_MIN_CM;
   }
   if (distance_cm > HCSR04_MAX_CM)
   {
       return HCSR04_MAX_CM;
   }
   return distance_cm;
}
