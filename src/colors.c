#include "colors.h"

uint32_t colors[3];

void colors_init()
{
    colors[COLOR_BG] = graphics_make_color(0x22, 0x23, 0x23, 0xff);
    colors[COLOR_BLACK] = 0;
    colors[COLOR_RED] = graphics_make_color(0xff, 0, 0, 0xff);
}