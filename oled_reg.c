#include "oled_reg.h"
#include "delay_tim2.h"
#include <stdio.h>
static void I2C1_WriteByte(uint8_t control, uint8_t data);
static void OLED_Command(uint8_t cmd);
static void OLED_Data(uint8_t data);
static void OLED_SetCursor(uint8_t page, uint8_t col);
static void OLED_Clear(void);
static void OLED_Char(char c);
static void OLED_String(const char *s);
void I2C1_Init_Reg(void)
{
   RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
   I2C1->CR1 &= ~I2C_CR1_PE;
   I2C1->CR2  = 16U;
   I2C1->CCR  = 80U;
   I2C1->TRISE = 17U;
   I2C1->CR1 |= I2C_CR1_PE;
}
static void I2C1_WriteByte(uint8_t control, uint8_t data)
{
   I2C1->CR1 |= I2C_CR1_START;
   while ((I2C1->SR1 & I2C_SR1_SB) == 0U) {}
   (void)I2C1->SR1;
   I2C1->DR = (OLED_ADDR << 1);
   while ((I2C1->SR1 & I2C_SR1_ADDR) == 0U) {}
   (void)I2C1->SR1;
   (void)I2C1->SR2;
   while ((I2C1->SR1 & I2C_SR1_TXE) == 0U) {}
   I2C1->DR = control;
   while ((I2C1->SR1 & I2C_SR1_TXE) == 0U) {}
   I2C1->DR = data;
   while ((I2C1->SR1 & I2C_SR1_BTF) == 0U) {}
   I2C1->CR1 |= I2C_CR1_STOP;
}
static void OLED_Command(uint8_t cmd)
{
   I2C1_WriteByte(0x00U, cmd);
}
static void OLED_Data(uint8_t data)
{
   I2C1_WriteByte(0x40U, data);
}
static void OLED_SetCursor(uint8_t page, uint8_t col)
{
   OLED_Command(0xB0U + page);
   OLED_Command(0x00U + (col & 0x0FU));
   OLED_Command(0x10U + ((col >> 4U) & 0x0FU));
}
static void OLED_Clear(void)
{
   for (uint8_t page = 0; page < 8U; page++)
   {
       OLED_SetCursor(page, 0U);
       for (uint8_t col = 0; col < 128U; col++)
       {
           OLED_Data(0x00U);
       }
   }
}
void OLED_Init_Reg(void)
{
   delay_ms(100U);
   OLED_Command(0xAE);   // Display OFF: tắt màn hình OLED trước khi cấu hình

   OLED_Command(0x20);   // Set Memory Addressing Mode: chọn chế độ địa chỉ bộ nhớ
   OLED_Command(0x00);   // Chọn Horizontal Addressing Mode

   OLED_Command(0xB0);   // Set Page Start Address = 0: bắt đầu từ page 0
   OLED_Command(0xC8);   // COM Output Scan Direction remapped: đảo chiều quét dọc để hiển thị đúng chiều

   OLED_Command(0x00);   // Set lower column address = 0: đặt 4 bit thấp của cột về 0
   OLED_Command(0x10);   // Set higher column address = 0: đặt 4 bit cao của cột về 0

   OLED_Command(0x40);   // Set Display Start Line = 0: dòng hiển thị bắt đầu từ line 0

   OLED_Command(0x81);   // Set Contrast Control: lệnh chỉnh độ tương phản
   OLED_Command(0x7F);   // Giá trị contrast = 0x7F (mức trung bình)

   OLED_Command(0xA1);   // Segment Re-map: đảo chiều trái/phải của màn hình
   OLED_Command(0xA6);   // Normal Display: hiển thị bình thường, không đảo màu

   OLED_Command(0xA8);   // Set Multiplex Ratio: cấu hình số hàng hiển thị
   OLED_Command(0x3F);   // Multiplex = 0x3F -> 64 hàng (phù hợp OLED 128x64)

   OLED_Command(0xA4);   // Display follows RAM content: hiển thị theo dữ liệu trong RAM OLED

   OLED_Command(0xD3);   // Set Display Offset: chỉnh độ lệch hiển thị theo chiều dọc
   OLED_Command(0x00);   // Offset = 0: không lệch

   OLED_Command(0xD5);   // Set Display Clock Divide Ratio / Oscillator Frequency
   OLED_Command(0x80);   // Giá trị clock mặc định/chuẩn cho SSD1306

   OLED_Command(0xD9);   // Set Pre-charge Period: cấu hình thời gian nạp trước cho pixel OLED
   OLED_Command(0xF1);   // Giá trị pre-charge phù hợp cho module OLED này

   OLED_Command(0xDA);   // Set COM Pins Hardware Configuration
   OLED_Command(0x12);   // Cấu hình COM pin cho màn hình 128x64

   OLED_Command(0xDB);   // Set VCOMH Deselect Level: chỉnh mức điện áp VCOMH
   OLED_Command(0x40);   // Giá trị VCOMH thường dùng cho SSD1306

   OLED_Command(0x8D);   // Charge Pump Setting: cấu hình mạch charge pump nội
   OLED_Command(0x14);   // Enable Charge Pump: bật charge pump để OLED hoạt động

   OLED_Command(0xAF);   // Display ON: bật màn hình OLED sau khi cấu hình xong

   OLED_Clear();         // Xóa toàn bộ màn hình, đưa về trạng thái trống
}
static void OLED_Char(char c)
{
   uint8_t font[5] = {0};
   switch (c)
   {
       case '0': font[0]=0x3E; font[1]=0x51; font[2]=0x49; font[3]=0x45; font[4]=0x3E; break;
       case '1': font[0]=0x00; font[1]=0x42; font[2]=0x7F; font[3]=0x40; font[4]=0x00; break;
       case '2': font[0]=0x42; font[1]=0x61; font[2]=0x51; font[3]=0x49; font[4]=0x46; break;
       case '3': font[0]=0x21; font[1]=0x41; font[2]=0x45; font[3]=0x4B; font[4]=0x31; break;
       case '4': font[0]=0x18; font[1]=0x14; font[2]=0x12; font[3]=0x7F; font[4]=0x10; break;
       case '5': font[0]=0x27; font[1]=0x45; font[2]=0x45; font[3]=0x45; font[4]=0x39; break;
       case '6': font[0]=0x3C; font[1]=0x4A; font[2]=0x49; font[3]=0x49; font[4]=0x30; break;
       case '7': font[0]=0x01; font[1]=0x71; font[2]=0x09; font[3]=0x05; font[4]=0x03; break;
       case '8': font[0]=0x36; font[1]=0x49; font[2]=0x49; font[3]=0x49; font[4]=0x36; break;
       case '9': font[0]=0x06; font[1]=0x49; font[2]=0x49; font[3]=0x29; font[4]=0x1E; break;
       case 'A': font[0]=0x7E; font[1]=0x11; font[2]=0x11; font[3]=0x11; font[4]=0x7E; break;
       case 'C': font[0]=0x3E; font[1]=0x41; font[2]=0x41; font[3]=0x41; font[4]=0x22; break;
       case 'D': font[0]=0x7F; font[1]=0x41; font[2]=0x41; font[3]=0x22; font[4]=0x1C; break;
       case 'E': font[0]=0x7F; font[1]=0x49; font[2]=0x49; font[3]=0x49; font[4]=0x41; break;
       case 'F': font[0]=0x7F; font[1]=0x09; font[2]=0x09; font[3]=0x09; font[4]=0x01; break;
       case 'G': font[0]=0x3E; font[1]=0x41; font[2]=0x49; font[3]=0x49; font[4]=0x7A; break;
       case 'I': font[0]=0x00; font[1]=0x41; font[2]=0x7F; font[3]=0x41; font[4]=0x00; break;
       case 'M': font[0]=0x7F; font[1]=0x02; font[2]=0x0C; font[3]=0x02; font[4]=0x7F; break;
       case 'N': font[0]=0x7F; font[1]=0x04; font[2]=0x08; font[3]=0x10; font[4]=0x7F; break;
       case 'O': font[0]=0x3E; font[1]=0x41; font[2]=0x41; font[3]=0x41; font[4]=0x3E; break;
       case 'R': font[0]=0x7F; font[1]=0x09; font[2]=0x19; font[3]=0x29; font[4]=0x46; break;
       case 'S': font[0]=0x46; font[1]=0x49; font[2]=0x49; font[3]=0x49; font[4]=0x31; break;
       case 'T': font[0]=0x01; font[1]=0x01; font[2]=0x7F; font[3]=0x01; font[4]=0x01; break;
       case 'W': font[0]=0x7F; font[1]=0x20; font[2]=0x18; font[3]=0x20; font[4]=0x7F; break;
       case ':': font[0]=0x00; font[1]=0x36; font[2]=0x36; font[3]=0x00; font[4]=0x00; break;
       case ' ': font[0]=0x00; font[1]=0x00; font[2]=0x00; font[3]=0x00; font[4]=0x00; break;
       default:  font[0]=0x00; font[1]=0x00; font[2]=0x00; font[3]=0x00; font[4]=0x00; break;
   }
   for (uint8_t i = 0; i < 5U; i++)
   {
       OLED_Data(font[i]);
   }
   OLED_Data(0x00U);
}
static void OLED_String(const char *s)
{
   while (*s)
   {
       OLED_Char(*s++);
   }
}
void OLED_Show(uint32_t distance, const char *status)
{
    char line[20];

    OLED_SetCursor(0U, 0U);
    OLED_String("MINI ADAS  ");

    OLED_SetCursor(1U, 0U);
    snprintf(line, sizeof(line), "DIST:%3luCM  ", (unsigned long)distance);
    OLED_String(line);

    OLED_SetCursor(2U, 0U);
    OLED_String("STATE:     ");

    OLED_SetCursor(3U, 0U);
    snprintf(line, sizeof(line), "%-10s", status);
    OLED_String(line);
}

