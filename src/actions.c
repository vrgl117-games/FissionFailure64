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
        .text = "PRESS GREEN\n\nBUTTON ON CENTER STATION",
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
        .text = "DO NOT PRESS GREEN\n\nBUTTON ON CENTER STATION",

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
        .text = "PRESS BLUE BUTTON ON CENTER\n\nSTATION AND SET GREEN LEVER\n\nTO RIGHT ON RIGHT STATION",

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
        .text = "DO NOT PRESS BLUE\n\nBUTTON ON CENTER STATION",

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
        .text = "SET GREEN LEVER TO\n\nLEFT ON RIGHT STATION",

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