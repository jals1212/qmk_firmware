/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "quantum.h"

#ifdef OLED_ENABLE
#    include "lib/host_led_state_reader.h"
#    include "lib/layer_state_reader.h"
#    include "lib/oled_pet.h"
#    include "lib/rgb_matrix_state_reader.h"
#endif

#ifdef SWAP_HANDS_ENABLE
__attribute__((weak)) const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    // Left
    {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}},
    {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}},
    {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}},
    {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}},
    // Right
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}},
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}},
    {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}}};
#endif

#ifdef RGB_MATRIX_ENABLE
static void set_key_rgb(uint8_t row, uint8_t col, uint8_t led_min, uint8_t led_max, uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t index = g_led_config.matrix_co[row][col];

    if (index != NO_LED && index >= led_min && index < led_max) {
        rgb_matrix_set_color(index, red, green, blue);
    }
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    if (host_keyboard_led_state().caps_lock) {
        set_key_rgb(1, 1, led_min, led_max, RGB_RED);
        set_key_rgb(1, 2, led_min, led_max, RGB_RED);
        set_key_rgb(1, 3, led_min, led_max, RGB_RED);
        set_key_rgb(1, 4, led_min, led_max, RGB_RED);
        set_key_rgb(5, 5, led_min, led_max, RGB_RED);
        set_key_rgb(5, 4, led_min, led_max, RGB_RED);
        set_key_rgb(5, 3, led_min, led_max, RGB_RED);
        set_key_rgb(5, 2, led_min, led_max, RGB_RED);
    }

    return false;
}
#endif

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180; // flips the display 180 degrees if offhand
    }
    return rotation;
}

static void oled_render_status(void) {
    oled_write_ln_P(PSTR("CRKBD"), false);
    oled_write_P(PSTR("Layer: "), false);
    oled_write_ln(read_layer_state(), false);
    oled_write_ln(read_host_led_state(), false);
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }

    if (is_keyboard_left()) {
        oled_render_status();
    } else {
        static uint8_t prev_layer    = 255;
        uint8_t        current_layer = get_highest_layer(layer_state);
        if (prev_layer != current_layer) {
            oled_clear();
            prev_layer = current_layer;
        }
        if (current_layer == 3) {
            oled_render_rgb_matrix_state();
        } else {
            oled_render_pet();
        }
    }

    return false;
}
#endif
