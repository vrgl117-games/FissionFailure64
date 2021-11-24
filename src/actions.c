#include "actions.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "dfs.h"

action_t *actions;
uint8_t num_actions;

static uint8_t current = 0;

void actions_init()
{
    action_t _actions[] =
        {

            {
                .states = {
                    {
                        .station = STATION_CENTER,
                        .element = ELEMENT_GRID,
                        .expected = {COLOR_RED, 2, 1},
                    },
                },
                .num_states = 1,
                .text = dfs_load_sprites("/gfx/sprites/actions/rod_r_b3-%d.sprite"),
            },
            {
                .states = {
                    {
                        .station = STATION_LEFT,
                        .element = ELEMENT_RADIO,
                        .expected = {242},
                    },
                },
                .num_states = 1,
                .text = dfs_load_sprites("/gfx/sprites/actions/freq-242-%d.sprite"),

            },
            {
                .states = {
                    {
                        .station = STATION_CENTER,
                        .element = ELEMENT_GRID,
                        .expected = {COLOR_BLUE, 2, 1},
                    },
                },
                .num_states = 1,
                .text = dfs_load_sprites("/gfx/sprites/actions/rod_b_b3-%d.sprite"),
            },
            {
                .states = {
                    {
                        .station = STATION_RIGHT,
                        .element = ELEMENT_KEYPAD,
                        .expected = {3, 9, 0, 5, 1, 2, 7, 3},
                    },
                },
                .num_states = 1,
                .text = dfs_load_sprites("/gfx/sprites/actions/call-spare-%d.sprite"),
            },
            {
                .states = {
                    {
                        .station = STATION_CENTER,
                        .element = ELEMENT_B,
                        .expected = {1},
                    },
                },
                .num_states = 1,
                .text = dfs_load_sprites("/gfx/sprites/actions/power_off-%d.sprite"),
            },
            {
                .states = {
                    {
                        .station = STATION_CENTER,
                        .element = ELEMENT_B,
                        .expected = {0},
                    },
                },
                .num_states = 1,
                .text = dfs_load_sprites("/gfx/sprites/actions/power_on-%d.sprite"),
            },
            {
                .states = {
                    {
                        .station = STATION_CENTER,
                        .element = ELEMENT_GRID,
                        .expected = {COLOR_RED, 1, 2},
                    },
                },
                .num_states = 1,
                .text = dfs_load_sprites("/gfx/sprites/actions/rod_r_c2-%d.sprite"),

            },
            {
                .states = {
                    {
                        .station = STATION_CENTER,
                        .element = ELEMENT_GRID,
                        .expected = {COLOR_BLUE, 1, 2},
                    },
                },
                .num_states = 1,
                .text = dfs_load_sprites("/gfx/sprites/actions/rod_b_c2-%d.sprite"),

            },

        };

    num_actions = sizeof(_actions) / sizeof(_actions[0]);
    actions = malloc(num_actions * sizeof(action_t));
    memcpy(actions, &_actions, sizeof(_actions));

    actions_reset();
}

action_t *actions_get_current()
{
    return &actions[current];
}

bool actions_next()
{
    current++;
    return (current >= num_actions);
}

void actions_reset()
{
    current = 0;
}