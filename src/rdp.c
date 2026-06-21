#include "rdp.h"

extern uint32_t __width;
extern uint32_t __height;

static color_t color_from_graphics(uint32_t color)
{
    if (color == 0)
        return RGBA32(0, 0, 0, 0xff);
    return color_from_packed16(color & 0xffff);
}

void rdp_attach_display_context(display_context_t disp)
{
    rdpq_attach(disp, NULL);
}

void rdp_draw_filled_fullscreen(uint32_t color)
{
    rdp_draw_filled_rectangle_size(0, 0, __width, __height, color);
}

void rdp_draw_filled_rectangle_absolute(int x, int y, int xx, int yy, uint32_t color)
{
    rdpq_set_mode_fill(color_from_graphics(color));
    rdpq_fill_rectangle(x, y, xx, yy);
}

void rdp_draw_filled_rectangle_size(int x, int y, int width, int height, uint32_t color)
{
    rdp_draw_filled_rectangle_absolute(x, y, x + width, y + height, color);
}

void rdp_draw_sprite_with_texture(sprite_t *sp, int x, int y, mirror_t mirror)
{
    rdpq_set_mode_standard();
    rdpq_mode_alphacompare(1);
    rdpq_sprite_blit(sp, x, y, &(rdpq_blitparms_t){
        .flip_x = mirror == MIRROR_X,
        .flip_y = mirror == MIRROR_Y,
    });
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
