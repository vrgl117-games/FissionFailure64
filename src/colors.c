#include "colors.h"

uint32_t colors[COLOR_IDX];
uint32_t colors_dark[COLOR_IDX];

void colors_init()
{
    colors[COLOR_BLACK] = 0;
    colors[COLOR_RED] = graphics_make_color(0xff, 0, 0, 0xff);
    colors[COLOR_GREEN] = graphics_make_color(0x00, 0xff, 0x00, 0xff);
    colors[COLOR_BLUE] = graphics_make_color(0x00, 0x00, 0xff, 0xff);
    colors[COLOR_ORANGE] = graphics_make_color(0xff, 0x77, 0x00, 0xff);
    colors[COLOR_BG] = graphics_make_color(0x80, 0x8f, 0x99, 0xff);
    colors[COLOR_WHITE] = graphics_make_color(0xff, 0xff, 0xff, 0xff);

    colors[COLOR_PANEL] = graphics_make_color(0x33, 0x66, 0x55, 0xff);
    colors[COLOR_BORDER] = graphics_make_color(0x20, 0x30, 0x20, 0xff);
    colors[COLOR_BROWN] = graphics_make_color(0x66, 0x55, 0x44, 0xff);
    colors[COLOR_YELLOW] = graphics_make_color(0xcc, 0xcc, 0x00, 0xff);

    colors_dark[COLOR_BLACK] = 0;
    colors_dark[COLOR_RED] = graphics_make_color(0xaa, 0, 0, 0xff);
    colors_dark[COLOR_GREEN] = graphics_make_color(0x00, 0xaa, 0x00, 0xff);
    colors_dark[COLOR_BLUE] = graphics_make_color(0x00, 0x00, 0xaa, 0xff);
    colors_dark[COLOR_ORANGE] = graphics_make_color(0xaa, 0x77, 0x00, 0xaa);
    colors_dark[COLOR_WHITE] = graphics_make_color(0xaa, 0xaa, 0xaa, 0xff);
}