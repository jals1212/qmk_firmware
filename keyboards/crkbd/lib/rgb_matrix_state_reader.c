#ifdef OLED_ENABLE

#include QMK_KEYBOARD_H

extern uint8_t oled_timeout_mins;

#ifdef RGB_MATRIX_ENABLE
static const char *read_rgb_matrix_mode(void) {
    switch (rgb_matrix_get_mode()) {
        case RGB_MATRIX_SOLID_COLOR:
            return "Solid";
#    ifdef ENABLE_RGB_MATRIX_ALPHAS_MODS
        case RGB_MATRIX_ALPHAS_MODS:
            return "Alpha";
#    endif
#    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE
        case RGB_MATRIX_SOLID_REACTIVE:
            return "React";
#    endif
        default:
            return "?";
    }
}

void oled_render_rgb_matrix_state(void) {
    // Horizontal OLED: 21 chars per line, 4 lines tall.
    oled_write_P(PSTR("RGB:"), false);
    oled_write_P(rgb_matrix_is_enabled() ? PSTR("On") : PSTR("Off"), false);
    oled_write_P(PSTR("  Mode:"), false);
    oled_write_ln(read_rgb_matrix_mode(), false);

    oled_write_P(PSTR("Hue:"), false);
    oled_write(get_u8_str(rgb_matrix_get_hue(), ' '), false);
    oled_write_P(PSTR(" Sat:"), false);
    oled_write_ln(get_u8_str(rgb_matrix_get_sat(), ' '), false);

    oled_write_P(PSTR("Brt:"), false);
    oled_write(get_u8_str(rgb_matrix_get_val(), ' '), false);
    oled_write_P(PSTR(" Spd:"), false);
    oled_write_ln(get_u8_str(rgb_matrix_get_speed(), ' '), false);

    oled_write_P(PSTR("Out:"), false);
    if (oled_timeout_mins == 0) {
        oled_write_ln_P(PSTR("---"), false);
    } else {
        oled_write(get_u8_str(oled_timeout_mins, ' '), false);
        oled_write_ln_P(PSTR("m"), false);
    }
}
#else
void oled_render_rgb_matrix_state(void) {
    oled_write_ln_P(PSTR("RGB: Off"), false);
}
#endif

#endif
