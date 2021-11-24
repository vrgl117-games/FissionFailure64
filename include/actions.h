#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <stdbool.h>
#include <stdint.h>

#include <libdragon.h>

#include "dfs.h"

typedef struct state
{
    enum
    {
        STATION_LEFT,
        STATION_CENTER,
        STATION_RIGHT
    } station;

    enum
    {
        ELEMENT_GRID,
        ELEMENT_RADIO,
        ELEMENT_KEYPAD,
        ELEMENT_A,
        ELEMENT_B,
    } element;

    uint8_t expected[8];

} state_t;

typedef struct action
{
    state_t states[4];
    uint8_t num_states;
    sprites_t *text;

} action_t;

action_t *actions_get_current();
void actions_init();
bool actions_next();
void actions_reset();

#endif //__ACTIONS_H__