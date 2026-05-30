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
#include "transactions.h"

#ifdef OLED_ENABLE
#    include "lib/host_led_state_reader.h"
#    include "lib/layer_state_reader.h"
#    include "lib/rgb_matrix_state_reader.h"
#    ifdef ENABLE_PET
#        include "lib/oled_pet.h"
#    endif
#endif

uint8_t oled_timeout_mins = 10;

static void oled_timeout_sync_handler(uint8_t buflen, const void *data, uint8_t out_buflen, void *out_data) {
    oled_timeout_mins = *(const uint8_t *)data;
}

void keyboard_post_init_kb(void) {
    transaction_register_rpc(SYNC_OLED_TIMEOUT, oled_timeout_sync_handler);
    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    if (is_keyboard_master()) {
        static uint8_t last_sent = 255;
        if (last_sent != oled_timeout_mins) {
            if (transaction_rpc_send(SYNC_OLED_TIMEOUT, sizeof(oled_timeout_mins), &oled_timeout_mins)) {
                last_sent = oled_timeout_mins;
            }
        }
    }
    housekeeping_task_user();
}

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
    return is_keyboard_master() ? OLED_ROTATION_270 : OLED_ROTATION_180;
}

// "JALS" pre-centered in 32 px (4 px padding + 4 glyphs * 6 bytes + 4 px
// padding). Bytes taken from drivers/oled/glcdfont.c (chars J, A, L, S).
static const char PROGMEM jals[32] = {
    0x00, 0x00, 0x00, 0x00,
    0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, // J
    0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00, // A
    0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, // L
    0x26, 0x49, 0x49, 0x49, 0x32, 0x00, // S
    0x00, 0x00, 0x00, 0x00,
};

static void oled_render_status(void) {
    // Vertical OLED: 32 px wide, 16 lines tall.
    // Layout: JALS (0) | blank (1-2) | big digit (3-8, 32x48) | blank | Caps (14) | state (15)
    oled_set_cursor(0, 0);
    oled_write_raw_P(jals, sizeof(jals));
    oled_render_layer_state(3);
    oled_set_cursor(0, 14);
    oled_write_ln_P(PSTR("Caps"), false);
    oled_write_ln(read_host_led_state(), false);
}

bool oled_task_kb(void) {
    static bool sleeping             = false;
#ifdef RGB_MATRIX_ENABLE
    static bool rgb_was_on           = false;
#endif

    bool timed_out = oled_timeout_mins > 0
                  && last_matrix_activity_elapsed() > (uint32_t)oled_timeout_mins * 60000UL;

    if (timed_out) {
        if (!sleeping) {
            sleeping = true;
#ifdef RGB_MATRIX_ENABLE
            rgb_was_on = rgb_matrix_is_enabled();
            if (rgb_was_on) rgb_matrix_disable_noeeprom();
#endif
        }
        oled_off();
        return false;
    }

    if (sleeping) {
        sleeping = false;
#ifdef RGB_MATRIX_ENABLE
        if (rgb_was_on) rgb_matrix_enable_noeeprom();
#endif
    }
    oled_on();

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
#ifdef ENABLE_PET
            oled_render_pet();
            oled_set_cursor(0, 0);
            oled_write_P(PSTR("WPM "), false);
            oled_write(get_u8_str(get_current_wpm(), ' '), false);
#endif
        }
    }

    return false;
}
#endif
