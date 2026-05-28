#include QMK_KEYBOARD_H

const char *read_layer_state(void) {
    switch (get_highest_layer(layer_state)) {
        case 1:
            return "Lower ";
        case 2:
            return "Raise ";
        case 3:
            return "Adjust";
        default:
            return "Base  ";
    }
}
