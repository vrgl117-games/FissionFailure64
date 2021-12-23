
#ifndef __RDP_H__
#define __RDP_H__

#include <libdragon.h>

#include "dfs.h"

void rdp_attach(display_context_t disp);
void rdp_draw_filled_fullscreen(uint32_t color);
void rdp_draw_filled_rectangle_size(int x, int y, int width, int height, uint32_t color);
void rdp_draw_filled_rectangle_absolute(int x, int y, int xx, int yy, uint32_t color);
void rdp_draw_filled_rectangle_with_border_size(int x, int y, int width, int height, uint32_t color, uint32_t border_color);
void rdp_draw_sprite_with_texture(sprite_t *sp, int x, int y, mirror_t mirror);
int rdp_draw_sprites_int(int x, int y, sprites_t *sprites, int size, int n, mirror_t mirror);
void rdp_draw_sprites_with_texture(sprites_t *sprites, int x, int y, mirror_t mirror);

#endif //__RDP_H__