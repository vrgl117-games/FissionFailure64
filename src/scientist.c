#include "scientist.h"

#include <stdlib.h>

#include "control_panel.h"
#include "rdp.h"

extern control_panel_t control_panel;

static scientist_t scientists[NUM_SCIENTIST];
static sprites_t *dark = NULL;

void scientist_draw()
{
    for (uint8_t i = 0; i < NUM_SCIENTIST; i++)
    {
        if (scientists[i].x >= 0 && scientists[i].x < MAX_VISIBLE_X)
        {
            sprites_t *sp;
            if (control_panel.lights_off)
                sp = (scientists[i].mode == 2 ? scientists[i].sprites[3] : dark);
            else
                sp = scientists[i].sprites[scientists[i].mode];

            rdp_draw_sprites_with_texture(sp, scientists[i].x, scientists[i].y_offset + MAX(scientists[i].y, 8), scientists[i].h_direction);
        }
        else
            scientists[i].mode = control_panel.mode;
    }
}

void scientist_win()
{
    for (uint8_t i = 0; i < NUM_SCIENTIST; i++)
        rdp_draw_sprites_with_texture(scientists[i].sprites[0], 18 + 40 * i, 34 + scientists[i].y_offset + MAX(scientists[i].y, 8), i % 2);
}

void scientist_init()
{
    if (dark == NULL)
    {
        dark = dfs_load_sprites("/gfx/sprites/scientists/dark-%d.sprite");

        sprites_t *scientists_sp[4][4] = {
            {
                dfs_load_sprites("/gfx/sprites/scientists/idle0-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/stressed0-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/hell0-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/dark-hell0-%d.sprite"),

            },
            {
                dfs_load_sprites("/gfx/sprites/scientists/idle1-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/stressed1-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/hell1-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/dark-hell1-%d.sprite"),
            },
            {
                dfs_load_sprites("/gfx/sprites/scientists/idle2-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/stressed2-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/hell2-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/dark-hell2-%d.sprite"),
            },
            {
                dfs_load_sprites("/gfx/sprites/scientists/idle3-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/stressed3-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/hell3-%d.sprite"),
                dfs_load_sprites("/gfx/sprites/scientists/dark-hell3-%d.sprite"),
            },
        };

        for (uint8_t i = 0; i < NUM_SCIENTIST; i++)
        {
            scientists[i].h_direction = (rand() % 2);
            scientists[i].v_direction = (rand() % 2);
            scientists[i].x = (rand() % (MAX_X - MIN_X)) + MIN_X;
            scientists[i].y = 1 + (rand() % 7);
            scientists[i].y_offset = 28 + (rand() % 3 * 2);

            uint8_t num = i;
            if (i > 3)
                num = rand() % 4;

            scientists[i].sprites[0] = scientists_sp[num][0];
            scientists[i].sprites[1] = scientists_sp[num][1];
            scientists[i].sprites[2] = scientists_sp[num][2];
            scientists[i].sprites[3] = scientists_sp[num][3];
        }
    }
}

void scientist_reset()
{
    for (uint8_t i = 0; i < NUM_SCIENTIST; i++)
        scientists[i].mode = 0;
}

void scientist_timer()
{
    for (uint8_t i = 0; i < NUM_SCIENTIST; i++)
    {
        if (!scientists[i].h_direction)
            scientists[i].x += (scientists[i].mode + 1);
        else
            scientists[i].x -= (scientists[i].mode + 1);

        if (scientists[i].v_direction)
            scientists[i].y++;
        else
            scientists[i].y--;

        if (scientists[i].x > MAX_X)
            scientists[i].h_direction = !scientists[i].h_direction;
        if (scientists[i].x <= MIN_X)
            scientists[i].h_direction = !scientists[i].h_direction;

        if (scientists[i].y > MAX_Y)
            scientists[i].v_direction = !scientists[i].v_direction;
        if (scientists[i].y <= 0)
            scientists[i].v_direction = !scientists[i].v_direction;
    }
}