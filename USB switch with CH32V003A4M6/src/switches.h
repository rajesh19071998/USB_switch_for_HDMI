/* Public interface for the five active-low switch inputs. */
#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdint.h>

#define SWITCH_COUNT 5U

/* Configure all five switch inputs with internal pull-up resistors. */
void switches_init(void);

/* Return the debounced state after three matching samples; low GPIO means on. */
uint8_t switches_is_on(uint8_t switch_index);

#endif /* SWITCHES_H */