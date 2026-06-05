#include "board.h"
#include "delay_tim2.h"
#include "indicators_reg.h"
#include "motor_reg.h"
#include "hcsr04_reg.h"
#include "oled_reg.h"
#include "app_adas.h"
#include "system_reg.h"

int main(void)
{
    Clock_HSI16_Init();
    GPIO_Init_Reg();
    TIM2_Delay_Init();
    TIM3_PWM_Init();
    TIM4_PWM_Init();
    I2C1_Init_Reg();
    OLED_Init_Reg();

    App_ADAS_Init();

    while (1)
    {
    	 App_ADAS_Run();
    }
}
