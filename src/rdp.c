#include "rdp.h"

extern uint32_t __width;
extern uint32_t __height;

static bool filled = false; //filled or texture_copy

void rdp_attach(display_context_t disp)
{
    rdp_attach_display(disp);
    rdp_set_default_clipping();
}

void rdp_draw_filled_fullscreen(uint32_t color)
{
    rdp_draw_filled_rectangle_size(0, 0, __width, __height, color);
}

void rdp_draw_filled_rectangle_absolute(int x, int y, int xx, int yy, uint32_t color)
{
    if (!filled)
    {
        rdp_enable_primitive_fill();
        filled = true;
    }
    rdp_sync(SYNC_PIPE);
    rdp_set_primitive_color(color);
    rdp_draw_filled_rectangle(x, y, xx, yy);
}

void rdp_draw_filled_rectangle_size(int x, int y, int width, int height, uint32_t color)
{
    if (!filled)
    {
        rdp_enable_primitive_fill();
        filled = true;
    }
    rdp_sync(SYNC_PIPE);
    rdp_set_primitive_color(color);
    rdp_draw_filled_rectangle(x, y, x + width, y + height);
}

void rdp_draw_filled_rectangle_with_border_size(int x, int y, int width, int height, uint32_t color, uint32_t border_color)
{
    if (!filled)
    {
        rdp_enable_primitive_fill();
        filled = true;
    }
    rdp_sync(SYNC_PIPE);
    rdp_set_primitive_color(border_color);
    rdp_draw_filled_rectangle(x, y, x + width, y + height);
    rdp_set_primitive_color(color);
    rdp_draw_filled_rectangle(x + 2, y + 2, x + width - 2, y + height - 2);
}

void rdp_draw_sprite_with_texture(sprite_t *sp, int x, int y, mirror_t mirror)
{
    if (filled)
    {
        rdp_enable_texture_copy();
        filled = false;
    }
    rdp_sync(SYNC_PIPE);
    rdp_load_texture(0, 0, mirror, sp);
    rdp_draw_sprite(0, x, y, mirror);
}

static int _rdp_draw_sprites_int(int x, int y, sprites_t *sprites, int n, mirror_t mirror)
{
    if (n >= 10)
        x = _rdp_draw_sprites_int(x, y, sprites, n / 10, mirror);

    rdp_draw_sprite_with_texture(sprites->sprites[n % 10], x, y, mirror);
    return x + 8;
}

int rdp_draw_sprites_int(int x, int y, sprites_t *sprites, int size, int n, mirror_t mirror)
{
    int count = 0;
    if (n == 0)
        count = 1;
    else
    {
        int nn = n;
        while (nn > 0)
        {
            nn /= 10;
            count++;
        }
    }
    size -= count;
    while (size > 0)
    {
        x = _rdp_draw_sprites_int(x, y, sprites, 0, mirror);
        size--;
    }
    return _rdp_draw_sprites_int(x, y, sprites, n, mirror);
}

void rdp_draw_sprites_with_texture(sprites_t *sprites, int x, int y, mirror_t mirror)
{
    int offset = 0;

    for (int i = 0; i < sprites->slices; i++)
    {
        int ii = (mirror == MIRROR_X ? sprites->slices - 1 - i : i);

        rdp_draw_sprite_with_texture(sprites->sprites[ii], x + offset, y, mirror);
        offset += sprites->sprites[ii]->width;
    }
}