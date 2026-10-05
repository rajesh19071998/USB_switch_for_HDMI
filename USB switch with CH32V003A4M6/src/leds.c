/* LED GPIO configuration and output control implementation. */
#include "leds.h"
#include "gpio_config.h"

#include <ch32v00x_gpio.h>
#include <ch32v00x_rcc.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} led_pin_t;

static const led_pin_t led_pins[LED_COUNT] = {
    {LED_1_PORT, LED_1_PIN},
    {LED_2_PORT, LED_2_PIN},
    {LED_3_PORT, LED_3_PIN},
    {LED_4_PORT, LED_4_PIN},
    {LED_5_PORT, LED_5_PIN}
};

void leds_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD, ENABLE);

    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_10MHz;

    gpio.GPIO_Pin = LED_GPIOC_PINS;
    GPIO_Init(GPIOC, &gpio);
    GPIO_ResetBits(GPIOC, gpio.GPIO_Pin);

    gpio.GPIO_Pin = LED_GPIOA_PINS;
    GPIO_Init(GPIOA, &gpio);
    GPIO_ResetBits(GPIOA, gpio.GPIO_Pin);

    gpio.GPIO_Pin = LED_GPIOD_PINS;
    GPIO_Init(GPIOD, &gpio);
    GPIO_ResetBits(GPIOD, gpio.GPIO_Pin);
}

void leds_set(uint8_t led_index, uint8_t on)
{
    if (led_index >= LED_COUNT)
    {
        return;
    }

    GPIO_WriteBit(led_pins[led_index].port, led_pins[led_index].pin,
                  on != 0U ? Bit_SET : Bit_RESET);
}