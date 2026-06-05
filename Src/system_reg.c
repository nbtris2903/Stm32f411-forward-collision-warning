#include "system_reg.h"

void Clock_HSI16_Init(void)
{
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0U)
    {
    }

    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    {
    }
}

void GPIO_Init_Reg(void)
{
    /* Enable GPIO clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* =========================
       GPIOA
       PA0  -> TRIG         output
       PA1  -> ECHO         input
       PA6  -> LED_YELLOW   output
       PA7  -> LED_RED      output
       PA8  -> IN2          output
       PA9  -> IN1          output
       ========================= */
    GPIOA->MODER &= ~((3U << (TRIG_PIN * 2U)) |
                      (3U << (ECHO_PIN * 2U)) |
                      (3U << (LED_YELLOW_PIN * 2U)) |
                      (3U << (LED_RED_PIN * 2U)) |
                      (3U << (IN2_PIN * 2U)) |
                      (3U << (IN1_PIN * 2U)));

    GPIOA->MODER |=  ((1U << (TRIG_PIN * 2U)) |
                      (1U << (LED_YELLOW_PIN * 2U)) |
                      (1U << (LED_RED_PIN * 2U)) |
                      (1U << (IN2_PIN * 2U)) |
                      (1U << (IN1_PIN * 2U)));
    /* PA1 remains input */

    GPIOA->OTYPER &= ~((1U << TRIG_PIN) |
                       (1U << LED_YELLOW_PIN) |
                       (1U << LED_RED_PIN) |
                       (1U << IN2_PIN) |
                       (1U << IN1_PIN));

    GPIOA->PUPDR &= ~((3U << (TRIG_PIN * 2U)) |
                      (3U << (ECHO_PIN * 2U)) |
                      (3U << (LED_YELLOW_PIN * 2U)) |
                      (3U << (LED_RED_PIN * 2U)) |
                      (3U << (IN2_PIN * 2U)) |
                      (3U << (IN1_PIN * 2U)));

    /* =========================
       GPIOB
       PB4  -> IN4          output
       PB5  -> BUZZER       output
       PB6  -> ENA          AF2 TIM4_CH1
       PB8  -> OLED_SCL     AF4 I2C1
       PB9  -> OLED_SDA     AF4 I2C1
       PB10 -> IN3          output
       ========================= */
    GPIOB->MODER &= ~((3U << (IN4_PIN * 2U)) |
                      (3U << (BUZZER_PIN * 2U)) |
                      (3U << (ENA_PIN * 2U)) |
                      (3U << (OLED_SCL_PIN * 2U)) |
                      (3U << (OLED_SDA_PIN * 2U)) |
                      (3U << (IN3_PIN * 2U)));

    /* Output: PB4, PB5, PB10 */
    GPIOB->MODER |=  ((1U << (IN4_PIN * 2U)) |
                      (1U << (BUZZER_PIN * 2U)) |
                      (1U << (IN3_PIN * 2U)));

    /* AF: PB6, PB8, PB9 */
    GPIOB->MODER |=  ((2U << (ENA_PIN * 2U)) |
                      (2U << (OLED_SCL_PIN * 2U)) |
                      (2U << (OLED_SDA_PIN * 2U)));

    GPIOB->OTYPER &= ~((1U << IN4_PIN) |
                       (1U << BUZZER_PIN) |
                       (1U << IN3_PIN));

    /* I2C open-drain */
    GPIOB->OTYPER |= (1U << OLED_SCL_PIN) | (1U << OLED_SDA_PIN);

    GPIOB->PUPDR &= ~((3U << (IN4_PIN * 2U)) |
                      (3U << (BUZZER_PIN * 2U)) |
                      (3U << (ENA_PIN * 2U)) |
                      (3U << (OLED_SCL_PIN * 2U)) |
                      (3U << (OLED_SDA_PIN * 2U)) |
                      (3U << (IN3_PIN * 2U)));

    /* Pull-up for I2C */
    GPIOB->PUPDR |= ((1U << (OLED_SCL_PIN * 2U)) |
                     (1U << (OLED_SDA_PIN * 2U)));

    /* Speed high for I2C */
    GPIOB->OSPEEDR |= ((3U << (OLED_SCL_PIN * 2U)) |
                       (3U << (OLED_SDA_PIN * 2U)));

    /* PB6 = AF2 TIM4_CH1 */
    GPIOB->AFR[0] &= ~(0xFU << (ENA_PIN * 4U));
    GPIOB->AFR[0] |=  (2U  << (ENA_PIN * 4U));

    /* PB8, PB9 = AF4 I2C1 */
    GPIOB->AFR[1] &= ~((0xFU << ((OLED_SCL_PIN - 8U) * 4U)) |
                       (0xFU << ((OLED_SDA_PIN - 8U) * 4U)));
    GPIOB->AFR[1] |=  ((4U << ((OLED_SCL_PIN - 8U) * 4U)) |
                       (4U << ((OLED_SDA_PIN - 8U) * 4U)));

    /* =========================
       GPIOC
       PC7 -> ENB AF2 TIM3_CH2
       ========================= */
    GPIOC->MODER &= ~(3U << (ENB_PIN * 2U));
    GPIOC->MODER |=  (2U << (ENB_PIN * 2U));

    GPIOC->PUPDR &= ~(3U << (ENB_PIN * 2U));

    GPIOC->AFR[0] &= ~(0xFU << (ENB_PIN * 4U));
    GPIOC->AFR[0] |=  (2U << (ENB_PIN * 4U));

    /* Initial output levels */
    PIN_CLR(TRIG_GPIO, TRIG_PIN);
    PIN_CLR(LED_YELLOW_GPIO, LED_YELLOW_PIN);
    PIN_CLR(LED_RED_GPIO, LED_RED_PIN);

    PIN_CLR(IN1_GPIO, IN1_PIN);
    PIN_CLR(IN2_GPIO, IN2_PIN);
    PIN_CLR(IN3_GPIO, IN3_PIN);
    PIN_CLR(IN4_GPIO, IN4_PIN);

    PIN_CLR(BUZZER_GPIO, BUZZER_PIN);
}
