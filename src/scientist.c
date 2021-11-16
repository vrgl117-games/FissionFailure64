#include "scientist.h"

#include "rdp.h"

static scientist_t scientists[3];

void scientist_draw(uint8_t mode)
{
    for (uint8_t i = 0; i < 3; i++)
        rdp_draw_sprites_with_texture(scientists[i].sprites[mode], scientists[i].x, 28 + (i * 2) + MAX(scientists[i].y, 8), scientists[i].h_direction);
}

void scientist_init()
{
    scientists[0].h_direction = false;
    scientists[0].v_direction = true;
    scientists[0].sprites[0] = dfs_load_sprites("/gfx/sprites/scientist/idle-%d.sprite");
    scientists[0].sprites[1] = dfs_load_sprites("/gfx/sprites/scientist/stressed-%d.sprite");
    scientists[0].sprites[2] = dfs_load_sprites("/gfx/sprites/scientist/hell-%d.sprite");

    scientists[1].h_direction = true;
    scientists[1].v_direction = true;
    scientists[1].x = 50;
    scientists[1].y = MAX_Y;
    scientists[1].sprites[0] = dfs_load_sprites("/gfx/sprites/scientist/idle2-%d.sprite");
    scientists[1].sprites[1] = dfs_load_sprites("/gfx/sprites/scientist/stressed2-%d.sprite");
    scientists[1].sprites[2] = dfs_load_sprites("/gfx/sprites/scientist/hell2-%d.sprite");

    scientists[2].h_direction = false;
    scientists[2].v_direction = false;
    scientists[2].x = 180;
    scientists[2].y = 6;
    scientists[2].sprites[0] = dfs_load_sprites("/gfx/sprites/scientist/idle3-%d.sprite");
    scientists[2].sprites[1] = dfs_load_sprites("/gfx/sprites/scientist/stressed3-%d.sprite");
    scientists[2].sprites[2] = dfs_load_sprites("/gfx/sprites/scientist/hell3-%d.sprite");
}

void scientist_timer()
{
    for (uint8_t i = 0; i < 3; i++)
    {
        if (!scientists[i].h_direction)
            scientists[i].x++;
        else
            scientists[i].x--;

        if (scientists[i].v_direction)
            scientists[i].y++;
        else
            scientists[i].y--;

        if (scientists[i].x > MAX_X)
            scientists[i].h_direction = !scientists[i].h_direction;
        if (scientists[i].x <= 0)
            scientists[i].h_direction = !scientists[i].h_direction;

        if (scientists[i].y > MAX_Y)
            scientists[i].v_direction = !scientists[i].v_direction;
        if (scientists[i].y <= 0)
            scientists[i].v_direction = !scientists[i].v_direction;
    }
}