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
                .label = LABEL_A,
                .expected = true,
            },
        },
        .num_buttons = 1,
        .text = "TURN ON\n\nGREEN FAN",
    };

    action_t action1 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_A,
                .expected = false,
            },
        },
        .num_buttons = 1,
        .text = "TURN GREEN\n\nFAN BACK OFF",

    };

    action_t action2 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_B,
                .expected = true,
            },
            {
                .station = STATION_RIGHT,
                .label = LABEL_A,
                .expected = true,
            },
        },
        .num_buttons = 2,
        .text = "TURN ON BLUE\n\nFAN AND\n\nENGAGE GREEN\n\nTURBINE",

    };

    action_t action3 = {
        .buttons = {
            {
                .station = STATION_CENTER,
                .label = LABEL_B,
                .expected = false,
            },
        },
        .num_buttons = 1,
        .text = "TURN OFF\n\nBLUE FAN",

    };

    action_t action4 = {
        .buttons = {
            {
                .station = STATION_RIGHT,
                .label = LABEL_A,
                .expected = false,
            },
        },
        .num_buttons = 1,
        .text = "DISENGAGE\n\nGREEN\n\nTURBINE",

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