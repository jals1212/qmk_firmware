#pragma once

#include <stdint.h>

#ifdef OLED_ENABLE
// Renders the current layer (as a big digit) at (0, line) on the OLED.
void oled_render_layer_state(uint8_t line);
#endif
