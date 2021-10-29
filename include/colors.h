#ifndef __COLORS_H__
#define __COLORS_H__

#include <libdragon.h>

typedef enum
{
    COLOR_BG,
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_RED,
    COLOR_PANEL,
    COLOR_BORDER,
    COLOR_BROWN,
    COLOR_YELLOW,
} colors_name_t;

void colors_init();

#endif //__COLORS_H__