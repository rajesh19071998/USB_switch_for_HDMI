/* Active-low switch GPIO configuration and state-reading implementation. */
#include "switches.h"
#include "gpio_config.h"

#include <ch32v00x_gpio.h>
#include <ch32v00x_rcc.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} switch_pin_t;

#define SWITCH_DEBOUNCE_SAMPLES 3U

static const switch_pin_t switch_pins[SWITCH_COUNT] = {
    {SWITCH_1_PORT, SWITCH_1_PIN},
    {SWITCH_2_PORT, SWITCH_2_PIN},
    {SWITCH_3_PORT, SWITCH_3_PIN},
    {SWITCH_4_PORT, SWITCH_4_PIN},
    {SWITCH_5_PORT, SWITCH_5_PIN}
};
static uint8_t switch_stable_state[SWITCH_COUNT];
static uint8_t switch_candidate_state[SWITCH_COUNT];
static uint8_t switch_candidate_count[SWITCH_COUNT];

void switches_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Pin = SWITCH_GPIOC_PINS;
    GPIO_Init(GPIOC, &gpio);

    for (uint8_t index = 0U; index < SWITCH_COUNT; index++)
    {
        uint8_t is_on = GPIO_ReadInputDataBit(switch_pins[index].port,
                                              switch_pins[index].pin) == Bit_RESET;

        switch_stable_state[index] = is_on;
        switch_candidate_state[index] = is_on;
        switch_candidate_count[index] = 0U;
    }
}

uint8_t switches_is_on(uint8_t switch_index)
{
    if (switch_index >= SWITCH_COUNT)
    {
        return 0U;
    }

    uint8_t is_on = GPIO_ReadInputDataBit(switch_pins[switch_index].port,
                                          switch_pins[switch_index].pin) == Bit_RESET;

    if (is_on == switch_stable_state[switch_index])
    {
        switch_candidate_count[switch_index] = 0U;
    }
    else if (is_on != switch_candidate_state[switch_index])
    {
        switch_candidate_state[switch_index] = is_on;
        switch_candidate_count[switch_index] = 1U;
    }
    else if (++switch_candidate_count[switch_index] >= SWITCH_DEBOUNCE_SAMPLES)
    {
        switch_stable_state[switch_index] = is_on;
        switch_candidate_count[switch_index] = 0U;
    }

    return switch_stable_state[switch_index];
}