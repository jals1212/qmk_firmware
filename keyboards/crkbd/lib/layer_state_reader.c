#ifdef OLED_ENABLE

#include QMK_KEYBOARD_H
#include "layer_state_reader.h"

#define BIG_DIGIT_COUNT 4
#define GLYPH_W 5
#define GLYPH_H 7
#define BIG_SCALE 6
#define BIG_W 32
#define BIG_H 48
#define BIG_PAGES (BIG_H / 8)
#define BIG_BYTES (BIG_W * BIG_PAGES)
#define BIG_MARGIN_X ((BIG_W - GLYPH_W * BIG_SCALE) / 2)
#define BIG_MARGIN_Y ((BIG_H - GLYPH_H * BIG_SCALE) / 2)

static const uint8_t PROGMEM digit_font[BIG_DIGIT_COUNT][GLYPH_W] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x72, 0x49, 0x49, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x49, 0x4D, 0x33}, // 3
};

void oled_render_layer_state(uint8_t line) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer >= BIG_DIGIT_COUNT) return;

    uint8_t glyph[GLYPH_W];
    for (uint8_t i = 0; i < GLYPH_W; i++) {
        glyph[i] = pgm_read_byte(&digit_font[layer][i]);
    }

    uint8_t page_buf[BIG_W];
    for (uint8_t p = 0; p < BIG_PAGES; p++) {
        for (uint8_t c = 0; c < BIG_W; c++) {
            uint8_t byte = 0;
            if (c >= BIG_MARGIN_X && c < BIG_MARGIN_X + GLYPH_W * BIG_SCALE) {
                uint8_t gc = (c - BIG_MARGIN_X) / BIG_SCALE;
                uint8_t col_bits = glyph[gc];
                for (uint8_t b = 0; b < 8; b++) {
                    uint8_t r = p * 8 + b;
                    if (r < BIG_MARGIN_Y) continue;
                    if (r >= BIG_MARGIN_Y + GLYPH_H * BIG_SCALE) continue;
                    uint8_t gr = (r - BIG_MARGIN_Y) / BIG_SCALE;
                    if (col_bits & (1 << gr)) byte |= (1 << b);
                }
            }
            page_buf[c] = byte;
        }
        oled_set_cursor(0, line + p);
        oled_write_raw((const char *)page_buf, BIG_W);
    }
}

#endif
