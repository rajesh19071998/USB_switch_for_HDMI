/* Application loop that mirrors active-low switches to LEDs and the display. */
#include <ch32v00x.h>
#include <debug.h>

#include "leds.h"
#include "switches.h"
#include "tm1637.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void off_remaining_leds(uint8_t active_led)
{
    for (uint8_t index = 0U; index < LED_COUNT; index++)
    {
        if (index != active_led)
        {
            leds_set(index, 0);
        }
    }
}

int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();

    switches_init();
    leds_init();
    tm1637_init();

    while (1)
    {
        /* When several switches are on, display the lowest-numbered LED. */
        uint8_t active_led = LED_COUNT;



        if (switches_is_on(0))
        {
            leds_set(0,1);
            off_remaining_leds(0);
            active_led = 0;
        }
        else if (switches_is_on(1))
        {
            leds_set(1,1);
            off_remaining_leds(1);
            active_led = 1;
        }
        else if (switches_is_on(2))
        {
            leds_set(2,1);
            off_remaining_leds(2);
            active_led = 2;
        }
        else if (switches_is_on(3))
        {
            leds_set(3,1);
            off_remaining_leds(3);
            active_led = 3;
        }
        else if (switches_is_on(4))
        {
            leds_set(4,1);
            off_remaining_leds(4);
            active_led = 4;
        }
        else if (switches_is_on(5))
        {
            leds_set(5,1);
            off_remaining_leds(5);
            active_led = 5;
        }
        else
        {
            off_remaining_leds(LED_COUNT);
        }

#if 0
        for (uint8_t index = 0U; index < SWITCH_COUNT; index++)
        {
            uint8_t is_on = switches_is_on(index);

            leds_set(index, is_on);
            if ((is_on != 0U) && (active_led == LED_COUNT))
            {
                active_led = index;
            }
        }

#endif

        tm1637_show_led(active_led);
        Delay_Ms(10);
    }

    return 0;
}

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
    while (1)
    {
    }
}