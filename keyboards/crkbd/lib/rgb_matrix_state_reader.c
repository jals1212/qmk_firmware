#ifdef OLED_ENABLE

#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE
static const char *read_rgb_matrix_mode(void) {
    switch (rgb_matrix_get_mode()) {
        case RGB_MATRIX_SOLID_COLOR:
            return "Solid";
#    ifdef ENABLE_RGB_MATRIX_ALPHAS_MODS
        case RGB_MATRIX_ALPHAS_MODS:
            return "Alpha";
#    endif
#    ifdef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
        case RGB_MATRIX_GRADIENT_LEFT_RIGHT:
            return "Grad";
#    endif
#    ifdef ENABLE_RGB_MATRIX_BREATHING
        case RGB_MATRIX_BREATHING:
            return "Brth";
#    endif
#    ifdef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
        case RGB_MATRIX_CYCLE_LEFT_RIGHT:
            return "Cycle";
#    endif
#    ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
        case RGB_MATRIX_CYCLE_OUT_IN_DUAL:
            return "CycIO";
#    endif
#    ifdef ENABLE_RGB_MATRIX_HUE_BREATHING
        case RGB_MATRIX_HUE_BREATHING:
            return "HBrth";
#    endif
#    ifdef ENABLE_RGB_MATRIX_HUE_WAVE
        case RGB_MATRIX_HUE_WAVE:
            return "HWave";
#    endif
#    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
        case RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
            return "Reac1";
#    endif
#    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE
        case RGB_MATRIX_SOLID_REACTIVE:
            return "React";
#    endif
#    ifdef ENABLE_RGB_MATRIX_SOLID_SPLASH
        case RGB_MATRIX_SOLID_SPLASH:
            return "Splsh";
#    endif
#    ifdef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
        case RGB_MATRIX_SOLID_MULTISPLASH:
            return "MSpl";
#    endif
        default:
            return "?";
    }
}

void oled_render_rgb_matrix_state(void) {
    oled_write_P(PSTR("RGB Matrix: "), false);
    oled_write_ln_P(rgb_matrix_is_enabled() ? PSTR("ON") : PSTR("OFF"), false);

    oled_write_P(PSTR("Mode: "), false);
    oled_write_ln(read_rgb_matrix_mode(), false);

    oled_write_P(PSTR("Hue:"), false);
    oled_write(get_u8_str(rgb_matrix_get_hue(), ' '), false);
    oled_write_P(PSTR(" Sat:"), false);
    oled_write_ln(get_u8_str(rgb_matrix_get_sat(), ' '), false);

    oled_write_P(PSTR("Bright:"), false);
    oled_write(get_u8_str(rgb_matrix_get_val(), ' '), false);
    oled_write_P(PSTR(" Speed:"), false);
    oled_write_ln(get_u8_str(rgb_matrix_get_speed(), ' '), false);
}
#else
void oled_render_rgb_matrix_state(void) {
    oled_write_ln_P(PSTR("RGB: Off"), false);
}
#endif

#endif
