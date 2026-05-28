#include QMK_KEYBOARD_H

const char *read_host_led_state(void) {
#ifdef CAPS_WORD_ENABLE
    if (is_caps_word_on()) {
        return "Caps: Word";
    }
#endif
    return host_keyboard_led_state().caps_lock ? "Caps: On" : "Caps: Off";
}
