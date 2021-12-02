#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <stdbool.h>
#include <stdint.h>

#include <libdragon.h>

#include "dfs.h"

typedef struct action
{
    enum
    {
        STATION_LEFT,
        STATION_CENTER,
        STATION_RIGHT
    } station;

    enum
    {
        ELEMENT_RADIO,
        ELEMENT_COMPASS,
        ELEMENT_GRID,
        ELEMENT_LIGHTS,
        ELEMENT_PRESSURIZER,
        ELEMENT_TURBINES,
        ELEMENT_PUMPS,
        ELEMENT_KEYPAD,
    } element;

    uint16_t expected[8];
    sprites_t *text;

} action_t;

typedef struct action_pair
{
    action_t *top;
    action_t *bottom;
} action_pair_t;

action_pair_t actions_get_current();
void actions_init();
bool actions_next();
void actions_reset();

#endif //__ACTIONS_H__