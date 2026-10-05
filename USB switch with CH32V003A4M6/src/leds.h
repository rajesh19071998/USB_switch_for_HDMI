/* Public interface for the five LED output channels. */
#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

#define LED_COUNT 5U

/* Configure all five LED GPIOs as push-pull outputs and turn them off. */
void leds_init(void);

/* Set an LED on (GPIO high) or off (GPIO low). */
void leds_set(uint8_t led_index, uint8_t on);

#endif /* LEDS_H */