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
    char buffer[256];
    sprites_t *text;

} action_t;

typedef struct action_pair
{
    action_t *top;
    action_t *bottom;
} action_pair_t;

uint16_t actions_get_points();
action_pair_t actions_get_current();
void actions_init();
bool actions_next(uint8_t i);
void actions_reset();

#endif //__ACTIONS_H__