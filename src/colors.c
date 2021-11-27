#include "colors.h"

uint32_t colors[COLOR_IDX];
uint32_t colors_dark[COLOR_IDX];

void colors_init()
{
    colors[COLOR_BLACK] = graphics_make_color(0x1F, 0x20, 0x29, 0xff);
    colors[COLOR_RED] = graphics_make_color(0xeb, 0x4b, 0x4e, 0xff);
    colors[COLOR_GREEN] = graphics_make_color(0x6b, 0xc6, 0xa9, 0xff);
    colors[COLOR_BLUE] = graphics_make_color(0x7b, 0xb8, 0xe5, 0xff);
    colors[COLOR_ORANGE] = graphics_make_color(0xeb, 0x80, 0x38, 0xff);
    colors[COLOR_BG] = graphics_make_color(0x80, 0x8f, 0x99, 0xff);
    colors[COLOR_WHITE] = graphics_make_color(0xf0, 0xf1, 0xfa, 0xff);

    colors[COLOR_PANEL] = graphics_make_color(0x5b, 0x60, 0x7a, 0xff);
    colors[COLOR_BROWN] = graphics_make_color(0xcf, 0x9c, 0x36, 0xff);
    colors[COLOR_YELLOW] = graphics_make_color(0xf9, 0xd7, 0x2f, 0xff);

    colors[COLOR_DARK] = 0;

    colors_dark[COLOR_BLACK] = 0;
    colors_dark[COLOR_RED] = graphics_make_color(0xaa, 0, 0, 0xff);
    colors_dark[COLOR_GREEN] = graphics_make_color(0x2c, 0x75, 0x7a, 0xff);
    colors_dark[COLOR_BLUE] = graphics_make_color(0x2a, 0x47, 0xaa, 0xa3);
    colors_dark[COLOR_ORANGE] = graphics_make_color(0xaa, 0x4f, 0x11, 0xff);
    colors_dark[COLOR_WHITE] = graphics_make_color(0xda, 0xdc, 0xe8, 0xff);
}