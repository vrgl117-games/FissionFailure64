#ifndef __COLORS_H__
#define __COLORS_H__

#include <libdragon.h>

typedef enum
{
    COLOR_BLACK,
    COLOR_RED,
    COLOR_BLUE,
    COLOR_BG,
    COLOR_WHITE,
    COLOR_PANEL,
    COLOR_BORDER,
    COLOR_BROWN,
    COLOR_YELLOW,
    COLOR_ORANGE,

    COLOR_IDX,
} colors_name_t;

void colors_init();

#endif //__COLORS_H__