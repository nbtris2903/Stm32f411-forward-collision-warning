#include "motor_reg.h"

void TIM3_PWM_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    TIM3->PSC = 16U - 1U;
    TIM3->ARR = PWM_PERIOD;
    TIM3->CCR2 = 0U;

    TIM3->CCMR1 &= ~(TIM_CCMR1_CC2S | TIM_CCMR1_OC2M);
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE | (6U << 12U);

    TIM3->CCER |= TIM_CCER_CC2E;
    TIM3->EGR  |= TIM_EGR_UG;
    TIM3->CR1  |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void TIM4_PWM_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    TIM4->PSC = 16U - 1U;
    TIM4->ARR = PWM_PERIOD;
    TIM4->CCR1 = 0U;

    TIM4->CCMR1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_OC1M);
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE | (6U << 4U);

    TIM4->CCER |= TIM_CCER_CC1E;
    TIM4->EGR  |= TIM_EGR_UG;
    TIM4->CR1  |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void Set_Left_PWM(uint16_t pwm)
{
    if (pwm > PWM_PERIOD)
    {
        pwm = PWM_PERIOD;
    }
    TIM4->CCR1 = pwm;
}

void Set_Right_PWM(uint16_t pwm)
{
    if (pwm > PWM_PERIOD)
    {
        pwm = PWM_PERIOD;
    }
    TIM3->CCR2 = pwm;
}

void Car_Stop(void)
{
    PIN_CLR(IN1_GPIO, IN1_PIN);
    PIN_CLR(IN2_GPIO, IN2_PIN);
    PIN_CLR(IN3_GPIO, IN3_PIN);
    PIN_CLR(IN4_GPIO, IN4_PIN);

    Set_Left_PWM(0U);
    Set_Right_PWM(0U);
}

void Car_Brake(void)
{
    PIN_SET(IN1_GPIO, IN1_PIN);
    PIN_SET(IN2_GPIO, IN2_PIN);
    PIN_SET(IN3_GPIO, IN3_PIN);
    PIN_SET(IN4_GPIO, IN4_PIN);

    Set_Left_PWM(PWM_PERIOD);
    Set_Right_PWM(PWM_PERIOD);
}

void Car_Forward_Speed(uint8_t speed_percent)
{
    uint16_t pwm;

    if (speed_percent > 100U)
    {
        speed_percent = 100U;
    }

    pwm = (uint16_t)((speed_percent * PWM_PERIOD) / 100U);



    PIN_SET(IN1_GPIO, IN1_PIN);
    PIN_CLR(IN2_GPIO, IN2_PIN);

    PIN_SET(IN3_GPIO, IN3_PIN);
    PIN_CLR(IN4_GPIO, IN4_PIN);

    Set_Left_PWM(pwm);
    Set_Right_PWM(pwm);
}

void Car_Backward_Speed(uint8_t speed_percent)
{
    uint16_t pwm;

    if (speed_percent > 100U)
    {
        speed_percent = 100U;
    }

    pwm = (uint16_t)((speed_percent * PWM_PERIOD) / 100U);

    PIN_CLR(IN1_GPIO, IN1_PIN);
    PIN_SET(IN2_GPIO, IN2_PIN);

    PIN_CLR(IN3_GPIO, IN3_PIN);
    PIN_SET(IN4_GPIO, IN4_PIN);

    Set_Left_PWM(pwm);
    Set_Right_PWM(pwm);
}
