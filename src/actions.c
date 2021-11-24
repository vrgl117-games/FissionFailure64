#include "actions.h"

#include <stdint.h>

#include "colors.h"
#include "dfs.h"

action_t actions[NUM_ACTIONS];
static uint8_t current = 0;

void actions_init()
{

    action_t action0 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_GRID,
                .expected = {COLOR_RED, 2, 1},
            },
        },
        .num_buttons = 1,
        .text = dfs_load_sprites("/gfx/sprites/actions/rod_r_b3-%d.sprite"),
    };

    action_t action1 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_GRID,
                .expected = {COLOR_BLUE, 2, 1},
            },
        },
        .num_buttons = 1,
        .text = dfs_load_sprites("/gfx/sprites/actions/rod_b_b3-%d.sprite"),
    };

    action_t action2 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_GRID,
                .expected = {COLOR_RED, 1, 2},
            },
        },
        .num_buttons = 1,
        .text = dfs_load_sprites("/gfx/sprites/actions/rod_r_c2-%d.sprite"),

    };

    action_t action3 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_GRID,
                .expected = {COLOR_BLUE, 1, 2},
            },
        },
        .num_buttons = 1,
        .text = dfs_load_sprites("/gfx/sprites/actions/rod_b_c2-%d.sprite"),

    };

    actions[0] = action0;
    actions[1] = action1;
    actions[2] = action2;
    actions[3] = action3;

    actions_reset();
}

action_t *actions_get_current()
{
    return &actions[current];
}

bool actions_next()
{
    current++;
    return (current >= NUM_ACTIONS);
}

void actions_reset()
{
    current = 0;
}