#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dfs.h"

void dfs_free_sprites(sprites_t *sprites)
{
    if (sprites == NULL)
        return;

    for (int i = 0; sprites->sprites[i] != 0; i++)
        free(sprites->sprites[i]);
    free(sprites);
}

sprite_t *dfs_load_sprite(const char *const path)
{
    int fp = dfs_open(path);

    if (fp > 0)
    {
        int s = dfs_size(fp);
        sprite_t *sp = malloc(s);
        dfs_read(sp, 1, s, fp);
        dfs_close(fp);

        // Invalidate data associated with sprite in cache
        if (sp->bitdepth > 0)
            data_cache_hit_writeback_invalidate(sp->data, sp->width * sp->height * sp->bitdepth);
        else
            data_cache_hit_writeback_invalidate(sp->data, (sp->width * sp->height) >> 1);

        return sp;
    }

    return NULL;
}

sprite_t *dfs_load_spritef(const char *const format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    return dfs_load_sprite(buffer);
}

sprites_t *dfs_load_sprites(const char *const path)
{
    char buffer[256];

    sprites_t *data = calloc(1, sizeof(sprites_t));

    int i = 0;
    while (true)
    {
        sprintf(buffer, path, i);
        sprite_t *sp = dfs_load_sprite(buffer);
        if (sp == NULL)
            break;

        data->sprites[i] = sp;
        data->width += data->sprites[i]->width;
        i++;
    }
    data->slices = i;
    data->height = data->sprites[0]->height;
    return data;
}
sprites_t *dfs_load_sprites_by_frame(sprites_t *self, const char *const path)
{
    char buffer[256];
    sprites_t *data = self;
    if (data == NULL)
        data = calloc(1, sizeof(sprites_t));

    sprintf(buffer, path, data->loaded);
    sprite_t *sp = dfs_load_sprite(buffer);
    if (sp == NULL)
    {
        data->loaded = -1;
    }
    else
    {

        data->sprites[data->loaded] = sp;
        data->width += data->sprites[data->loaded]->width;
        data->loaded++;
        data->slices = data->loaded;
        data->height = data->sprites[0]->height;
    }

    return data;
}

int dfs_openf(const char *const format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    return dfs_open(buffer);
}
