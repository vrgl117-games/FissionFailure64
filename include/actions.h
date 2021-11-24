#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <stdbool.h>
#include <stdint.h>

#include <libdragon.h>

#include "dfs.h"

#define NUM_ACTIONS 4

#define LABEL_A 0
#define LABEL_B 1
#define LABEL_Z 2
#define LABEL_GRID 3

#define STATION_LEFT 0
#define STATION_CENTER 1
#define STATION_RIGHT 2

typedef struct button
{
    uint8_t station;
    uint8_t label;
    uint8_t expected[3];

} button_t;

typedef struct action
{
    button_t buttons[4];
    uint8_t num_buttons;
    sprites_t *text;

} action_t;

action_t *actions_get_current();
void actions_init();
bool actions_next();
void actions_reset();

#endif //__ACTIONS_H__