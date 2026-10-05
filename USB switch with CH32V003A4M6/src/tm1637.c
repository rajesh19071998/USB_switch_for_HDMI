/* TM1637 serial protocol and LED-status display implementation. */
#include "tm1637.h"
#include "gpio_config.h"

#include <ch32v00x_gpio.h>
#include <ch32v00x_rcc.h>

#define TM1637_CMD_AUTO       0x40U
#define TM1637_CMD_ADDRESS    0xC0U
#define TM1637_CMD_DISPLAY    0x8FU
#define TM1637_SEGMENT_L      0x38U
#define TM1637_SEGMENT_O      0x3FU
#define TM1637_SEGMENT_F      0x71U
#define TM1637_DIGIT_1        0x06U
#define TM1637_DIGIT_2        0x5BU
#define TM1637_DIGIT_3        0x4FU
#define TM1637_DIGIT_4        0x66U
#define TM1637_DIGIT_5        0x6DU
#define TM1637_LED_LABEL_COUNT 5U
#define TM1637_DISPLAY_UNKNOWN 0xFFU

static uint8_t tm1637_last_led = TM1637_DISPLAY_UNKNOWN;

static void tm1637_delay(void)
{
    for (volatile uint32_t count = 0U; count < 120U; count++)
    {
    }
}

static void tm1637_start(void)
{
    GPIO_SetBits(TM1637_DIO_PORT, TM1637_DIO_PIN);
    GPIO_SetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);
    tm1637_delay();
    GPIO_ResetBits(TM1637_DIO_PORT, TM1637_DIO_PIN);
    tm1637_delay();
    GPIO_ResetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);
}

static void tm1637_stop(void)
{
    GPIO_ResetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);
    GPIO_ResetBits(TM1637_DIO_PORT, TM1637_DIO_PIN);
    tm1637_delay();
    GPIO_SetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);
    tm1637_delay();
    GPIO_SetBits(TM1637_DIO_PORT, TM1637_DIO_PIN);
}

static uint8_t tm1637_write_byte(uint8_t value)
{
    for (uint8_t bit = 0U; bit < 8U; bit++)
    {
        GPIO_ResetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);

        if ((value & 0x01U) != 0U)
        {
            GPIO_SetBits(TM1637_DIO_PORT, TM1637_DIO_PIN);
        }
        else
        {
            GPIO_ResetBits(TM1637_DIO_PORT, TM1637_DIO_PIN);
        }

        tm1637_delay();
        GPIO_SetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);
        tm1637_delay();
        value >>= 1U;
    }

    GPIO_ResetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);
    GPIO_SetBits(TM1637_DIO_PORT, TM1637_DIO_PIN);
    tm1637_delay();
    GPIO_SetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);
    tm1637_delay();
    uint8_t acknowledged = GPIO_ReadInputDataBit(TM1637_DIO_PORT, TM1637_DIO_PIN) == Bit_RESET;
    GPIO_ResetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);

    return acknowledged;
}

static uint8_t tm1637_write_segments(const uint8_t segments[4])
{
    uint8_t acknowledged = 1U;

    tm1637_start();
    acknowledged &= tm1637_write_byte(TM1637_CMD_AUTO);
    tm1637_stop();

    tm1637_start();
    acknowledged &= tm1637_write_byte(TM1637_CMD_ADDRESS);
    for (uint8_t index = 0U; index < 4U; index++)
    {
        acknowledged &= tm1637_write_byte(segments[index]);
    }
    tm1637_stop();

    tm1637_start();
    acknowledged &= tm1637_write_byte(TM1637_CMD_DISPLAY);
    tm1637_stop();

    return acknowledged;
}

void tm1637_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed = GPIO_Speed_10MHz;

    gpio.GPIO_Pin = TM1637_CLK_PIN;
    GPIO_Init(TM1637_CLK_PORT, &gpio);

    gpio.GPIO_Pin = TM1637_DIO_PIN;
    GPIO_Init(TM1637_DIO_PORT, &gpio);

    GPIO_SetBits(TM1637_CLK_PORT, TM1637_CLK_PIN);
    GPIO_SetBits(TM1637_DIO_PORT, TM1637_DIO_PIN);
    tm1637_clear();
}

void tm1637_show_led(uint8_t led_index)
{
    static const uint8_t digit_segments[] = {
        TM1637_DIGIT_1,
        TM1637_DIGIT_2,
        TM1637_DIGIT_3,
        TM1637_DIGIT_4,
        TM1637_DIGIT_5
    };
    uint8_t segments[4] = {TM1637_SEGMENT_L, 0U, 0U, 0U};
    uint8_t display_led = led_index < TM1637_LED_LABEL_COUNT ? led_index : TM1637_LED_LABEL_COUNT;

    if (display_led == tm1637_last_led)
    {
        return;
    }

    if (display_led >= TM1637_LED_LABEL_COUNT)
    {
        static const uint8_t off[4] = {
            TM1637_SEGMENT_O,
            TM1637_SEGMENT_F,
            TM1637_SEGMENT_F,
            0U
        };
        tm1637_clear();
        /*
        if (tm1637_write_segments(off) != 0U)
        {
            tm1637_last_led = display_led;
        }
        */
        return;
    }

    segments[1] = digit_segments[display_led];
    if (tm1637_write_segments(segments) != 0U)
    {
        tm1637_last_led = display_led;
    }
}

void tm1637_clear(void)
{
    static const uint8_t blank[4] = {0U, 0U, 0U, 0U};

    tm1637_last_led = TM1637_DISPLAY_UNKNOWN;
    (void)tm1637_write_segments(blank);
}