#include <stdarg.h>
#include <string.h>
#include <malloc.h>

#include "graphics.h"

void graphics_draw_textf(display_context_t disp, int x, int y, const char *const format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    graphics_draw_text(disp, x + 1, y + 1, buffer);
}

void graphics_draw_textf_with_background(display_context_t disp, int x, int y, uint32_t color, const char *const format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    int size = vsprintf(buffer, format, args);
    va_end(args);

    graphics_draw_box(disp, x, y, size * 8 + 5, 13, color);
    graphics_draw_text(disp, x + 3, y + 3, buffer);
}
