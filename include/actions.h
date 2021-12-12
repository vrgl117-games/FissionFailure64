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
    ELEMENT_GRID,
    ELEMENT_PUMPS,

    // NORMAL
    ELEMENT_RADIO,
    ELEMENT_KEYPAD,

    // HARD
    ELEMENT_AZ5,

    ELEMENT_TUTORIAL,
} element_t;

typedef struct action
{
    element_t element;
    uint16_t expected[8];
    char buffer[256];
    sprites_t *text;
    sprite_t *text2;

    enum
    {
        SHOW_NONE,
        SHOW_STATION,
        SHOW_DANGER,
    } show;

} action_t;

typedef struct action_pair
{
    action_t *top;
    action_t *bottom;
} action_pair_t;

typedef enum level
{
    EASY = 0,
    NORMAL = 15,
    HARD = 40,
} level_t;

typedef action_t *(*action_new)();

uint16_t actions_get_points();
action_t *actions_new_lights();
action_t *actions_new_lights_tutorial();
action_pair_t actions_get_current();
bool actions_next();
void actions_reset();

// Tutorial
action_pair_t actions_get_current_tutorial();
bool actions_next_tutorial();
void action_init_tutorial();
void actions_reset_tutorial();

#endif //__ACTIONS_H__