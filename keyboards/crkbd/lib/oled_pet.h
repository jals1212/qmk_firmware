#pragma once

#ifdef OLED_ENABLE

#include "pets/bongocat.h"

// Header-only dispatcher. Single-pet today; if another pet is added,
// swap the include + alias here (or branch on a PET_* define) without
// touching crkbd.c.
static inline void oled_render_pet(void) {
    render_bongo_cat();
}

#endif
