#include "indicators_reg.h"
void LED_All_Off(void)
{
   PIN_CLR(GPIOA, LED_YELLOW_PIN);
   PIN_CLR(GPIOA, LED_RED_PIN);
}
void LED_Yellow_On(void)
{
   PIN_SET(GPIOA, LED_YELLOW_PIN);
   PIN_CLR(GPIOA, LED_RED_PIN);
}
void LED_Red_On(void)
{
   PIN_CLR(GPIOA, LED_YELLOW_PIN);
   PIN_SET(GPIOA, LED_RED_PIN);
}
void Buzzer_Off(void)
{
   PIN_CLR(GPIOB, BUZZER_PIN);
}
void Buzzer_On(void)
{
   PIN_SET(GPIOB, BUZZER_PIN);
}

