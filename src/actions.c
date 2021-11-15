#include "actions.h"

#include <stdint.h>

action_t actions[NUM_ACTIONS];
static uint8_t current = 0;

void actions_init()
{

    action_t action0 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_GRID,
                .expected = {1, 1},
            },
        },
        .num_buttons = 1,
        .text = "TURN ON\n\nRED ROD\n\nB 2",
    };

    action_t action1 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_GRID,
                .expected = {2, 2},
            },
        },
        .num_buttons = 1,
        .text = "TURN ON\n\nBLUE ROD\n\nC 3",
    };

    action_t action2 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_A,
                .expected = {3},
            },
        },
        .num_buttons = 1,
        .text = "SET\n\nTENSION\n\nROD TO\n\n3",

    };

    action_t action3 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_B,
                .expected = {1},
            },
        },
        .num_buttons = 1,
        .text = "START\n\nDECONTA\n\nMINATION",

    };

    action_t action4 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_B,
                .expected = {0},
            },
        },
        .num_buttons = 1,
        .text = "END\n\nDECONTA\n\nMINATION",

    };

    actions[0] = action0;
    actions[1] = action1;
    actions[2] = action2;
    actions[3] = action3;
    actions[4] = action4;

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