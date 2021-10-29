#include "colors.h"

uint32_t colors[8];

void colors_init()
{
    colors[COLOR_BG] = graphics_make_color(0x80, 0x80, 0x80, 0xff);
    colors[COLOR_BLACK] = 0;
    colors[COLOR_WHITE] = graphics_make_color(0xff, 0xff, 0xff, 0xff);
    colors[COLOR_RED] = graphics_make_color(0xff, 0, 0, 0xff);
    colors[COLOR_PANEL] = graphics_make_color(0x33, 0x66, 0x55, 0xff);
    colors[COLOR_BORDER] = graphics_make_color(0x20, 0x30, 0x20, 0xff);
    colors[COLOR_BROWN] = graphics_make_color(0x66, 0x55, 0x44, 0xff);
    colors[COLOR_YELLOW] = graphics_make_color(0xff, 0xff, 0x00, 0xff);
}