#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__
#include <stdarg.h>

#include <libdragon.h>

typedef struct sprites
{
    sprite_t *sprites[6];
    int slices;

    int height;
    int width;

    int8_t loaded;
} sprites_t;

void dfs_free_sprites(sprites_t *sprites);
sprite_t *dfs_load_sprite(const char *const path);
sprite_t *dfs_load_spritef(const char *const format, ...);
sprites_t *dfs_load_sprites(const char *const path);
sprites_t *dfs_load_sprites_by_frame(sprites_t *self, const char *const path);
int dfs_openf(const char *const format, ...);

#endif //__FILESYSTEM_H__