#include "app_adas.h"
#include "delay_tim2.h"
#include "indicators_reg.h"
#include "motor_reg.h"
#include "hcsr04_reg.h"
#include "oled_reg.h"
#define SAFE_PWM_PERCENT       50U
#define WARNING_PWM_MIN        30U
#define WARNING_PWM_MAX        40U
#define DANGER_PWM_PERCENT     0U
void App_ADAS_Init(void)
{
   Buzzer_Off();
   LED_All_Off();
   Car_Stop();
   OLED_Show(0U, "START");
   delay_ms(300U);
}
void App_ADAS_Run(void)
{
   static float distance_filtered = 200.0f;
   float raw_distance;
   int dist_cm;
   int speed_percent;
   const char *state_txt = "";
   raw_distance = (float)HCSR04_Read_Reg();
   if (raw_distance < 2.0f || raw_distance > 400.0f)
   {
       raw_distance = 400.0f;
   }
   distance_filtered = 0.7f * distance_filtered + 0.3f * raw_distance;
   dist_cm = (int)(distance_filtered + 0.5f);
   if (distance_filtered > SAFE_DISTANCE_CM)
   {
       state_txt = "SAFE";
       Car_Forward_Speed(SAFE_PWM_PERCENT);
       Buzzer_Off();
       LED_All_Off();
       OLED_Show((uint32_t)dist_cm, state_txt);
       delay_ms(50U);
   }
   else if (distance_filtered >= WARNING_DISTANCE_CM)
   {
       state_txt = "WARNING";
       speed_percent = WARNING_PWM_MIN +
                       (int)((distance_filtered - WARNING_DISTANCE_CM) *
                       (WARNING_PWM_MAX - WARNING_PWM_MIN) /
                       (SAFE_DISTANCE_CM - WARNING_DISTANCE_CM));
       if (speed_percent < WARNING_PWM_MIN)
       {
           speed_percent = WARNING_PWM_MIN;
       }
       if (speed_percent > WARNING_PWM_MAX)
       {
           speed_percent = WARNING_PWM_MAX;
       }
       Car_Forward_Speed((uint8_t)speed_percent);
       Buzzer_Off();
       LED_All_Off();
       LED_Yellow_On();
       OLED_Show((uint32_t)dist_cm, state_txt);
       delay_ms(50U);
   }
   else
   {
       state_txt = "DANGER";
       Car_Brake();
       delay_ms(120U);
       Car_Stop();
       LED_All_Off();
       LED_Red_On();
       OLED_Show((uint32_t)dist_cm, state_txt);
       Buzzer_On();
       delay_ms(80U);
       Buzzer_Off();
       delay_ms(80U);
   }
}

