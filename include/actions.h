#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <stdbool.h>
#include <stdint.h>

#include <libdragon.h>

#include "dfs.h"

typedef enum element
{
    // EASY
    ELEMENT_TURBINES,
    ELEMENT_COMPASS,
    ELEMENT_PRESSURIZER,
    // NORMAL
    ELEMENT_RADIO,
    ELEMENT_GRID,
    ELEMENT_PUMPS,
    // HARD
    ELEMENT_KEYPAD,
    ELEMENT_AZ5,

    ELEMENT_IDX,
} element_t;

typedef struct action
{
    element_t element;
    uint16_t expected[8];
    char buffer[256];
    sprites_t *text;

} action_t;

typedef struct action_pair
{
    action_t *top;
    action_t *bottom;
} action_pair_t;

typedef enum level
{
    EASY = 10,
    NORMAL = 20,
    HARD = 30,
    EXTRA_HARD = 40,
} level_t;

typedef action_t *(*action_new)();

uint16_t actions_get_points();
action_pair_t actions_get_current();
void actions_init();
bool actions_next();
void actions_reset();

#endif //__ACTIONS_H__