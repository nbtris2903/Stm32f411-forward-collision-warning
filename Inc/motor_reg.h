#ifndef MOTOR_REG_H
#define MOTOR_REG_H

#include "board.h"
#include <stdint.h>

void TIM3_PWM_Init(void);
void TIM4_PWM_Init(void);

void Set_Left_PWM(uint16_t pwm);
void Set_Right_PWM(uint16_t pwm);

void Car_Stop(void);
void Car_Brake(void);
void Car_Forward_Speed(uint8_t speed_percent);
void Car_Backward_Speed(uint8_t speed_percent);

#endif
