#include QMK_KEYBOARD_H

const char *read_host_led_state(void) {
    return host_keyboard_led_state().caps_lock ? "Caps: ON " : "Caps: off";
}
