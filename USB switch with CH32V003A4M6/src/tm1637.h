/* Public interface for the TM1637 four-digit status display. */
#ifndef TM1637_H
#define TM1637_H

#include <stdint.h>

/* TM1637 CLK is PC1 and DIO is PA2. */
void tm1637_init(void);

/* Display L1 through L5 for LED indexes 0 through 4. */
void tm1637_show_led(uint8_t led_index);

/* Clear all four display positions. */
void tm1637_clear(void);

#endif /* TM1637_H */