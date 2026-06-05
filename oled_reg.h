#ifndef OLED_REG_H
#define OLED_REG_H
#include "board.h"
void I2C1_Init_Reg(void);
void OLED_Init_Reg(void);
void OLED_Show(uint32_t distance, const char *status);
#endif
