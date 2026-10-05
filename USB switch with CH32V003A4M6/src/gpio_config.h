/* Central GPIO pin assignments for the switch, LED, and TM1637 modules. */
#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include <ch32v00x_gpio.h>

/* Active-low switch inputs with internal pull-up resistors. */
#define SWITCH_1_PORT        GPIOC       /* SW1: PC1 */
#define SWITCH_1_PIN         GPIO_Pin_1
#define SWITCH_2_PORT        GPIOC       /* SW2: PC2 */
#define SWITCH_2_PIN         GPIO_Pin_2
#define SWITCH_3_PORT        GPIOC       /* SW3: PC3 */
#define SWITCH_3_PIN         GPIO_Pin_3
#define SWITCH_4_PORT        GPIOC       /* SW4: PC4 */
#define SWITCH_4_PIN         GPIO_Pin_4
#define SWITCH_5_PORT        GPIOC       /* SW5: PC6 */
#define SWITCH_5_PIN         GPIO_Pin_6
#define SWITCH_GPIOC_PINS    (SWITCH_1_PIN | SWITCH_2_PIN | SWITCH_3_PIN | SWITCH_4_PIN | SWITCH_5_PIN)

/* LED outputs driven high when their corresponding switch is on. */
#define LED_1_PORT           GPIOC       /* LED1: PC0 */
#define LED_1_PIN            GPIO_Pin_0
#define LED_2_PORT           GPIOA       /* LED2: PA2 */
#define LED_2_PIN            GPIO_Pin_2
#define LED_3_PORT           GPIOA       /* LED3: PA1 */
#define LED_3_PIN            GPIO_Pin_1
#define LED_4_PORT           GPIOD       /* LED4: PD6 */
#define LED_4_PIN            GPIO_Pin_6
#define LED_5_PORT           GPIOD       /* LED5: PD5 */
#define LED_5_PIN            GPIO_Pin_5
#define LED_GPIOC_PINS       LED_1_PIN
#define LED_GPIOA_PINS       (LED_2_PIN | LED_3_PIN)
#define LED_GPIOD_PINS       (LED_4_PIN | LED_5_PIN)

/* TM1637 four-digit display interface. */
#define TM1637_CLK_PORT      GPIOC       /* TM1637 CLK: PC7 */
#define TM1637_CLK_PIN       GPIO_Pin_7
#define TM1637_DIO_PORT      GPIOD       /* TM1637 DIO: PD4 */
#define TM1637_DIO_PIN       GPIO_Pin_4

#endif /* GPIO_CONFIG_H */