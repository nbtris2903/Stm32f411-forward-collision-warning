#ifndef BOARD_H
#define BOARD_H

#include "stm32f411xe.h"
#include <stdint.h>

/* Distance thresholds */
#define SAFE_DISTANCE_CM          100.0f
#define WARNING_DISTANCE_CM        50.0f

/* PWM */
#define PWM_PERIOD                999U

/* OLED */
#define OLED_ADDR                 0x3C

/* HC-SR04 */
#define TRIG_GPIO                 GPIOA
#define TRIG_PIN                  0U
#define ECHO_GPIO                 GPIOA
#define ECHO_PIN                  1U

/* LEDs */
#define LED_YELLOW_GPIO           GPIOA
#define LED_YELLOW_PIN            6U
#define LED_RED_GPIO              GPIOA
#define LED_RED_PIN               7U

/* Motor direction  */
#define IN1_GPIO                  GPIOA
#define IN1_PIN                   9U
#define IN2_GPIO                  GPIOA
#define IN2_PIN                   8U
#define IN3_GPIO                  GPIOB
#define IN3_PIN                   10U
#define IN4_GPIO                  GPIOB
#define IN4_PIN                   4U

/* Motor enable PWM  */
#define ENA_GPIO                  GPIOB
#define ENA_PIN                   6U
#define ENB_GPIO                  GPIOC
#define ENB_PIN                   7U

/* Buzzer */
#define BUZZER_GPIO               GPIOB
#define BUZZER_PIN                5U

/* OLED I2C */
#define OLED_SCL_GPIO             GPIOB
#define OLED_SCL_PIN              8U
#define OLED_SDA_GPIO             GPIOB
#define OLED_SDA_PIN              9U

#define PIN_SET(GPIOx, pin)       ((GPIOx)->BSRR = (1U << (pin)))
#define PIN_CLR(GPIOx, pin)       ((GPIOx)->BSRR = (1U << ((pin) + 16U)))

#endif
