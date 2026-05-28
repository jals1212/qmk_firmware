#ifdef OLED_ENABLE

#include QMK_KEYBOARD_H

void oled_render_pet(void) {
    if ((timer_read32() / 1000) & 1) {
        oled_write_ln_P(PSTR(" /\\_/\\"), false);
        oled_write_ln_P(PSTR("( o.o )"), false);
        oled_write_ln_P(PSTR(" > ^ <"), false);
    } else {
        oled_write_ln_P(PSTR(" /\\_/\\"), false);
        oled_write_ln_P(PSTR("( -.- ) zZz"), false);
        oled_write_ln_P(PSTR(" > ^ <"), false);
    }
}

#endif
