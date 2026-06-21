#include "actions.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "control_panel.h"
#include "dfs.h"

static uint16_t points = 0;

static action_pair_t pair = {0};

// Last rod action
static int8_t color_prev = -1;
static int8_t pos_x_prev = -1;
static int8_t pos_y_prev = -1;

extern control_panel_t control_panel;

// actions for left station
static action_t *actions_new_freq()
{
    action_t *action = calloc(1, sizeof(action_t));
    uint16_t freq = (rand() % 125) * 5;

    while (freq == control_panel.freq)
        freq = (rand() % 125) * 5;

    action->element = ELEMENT_RADIO;
    action->expected[0] = freq;
    sprintf(action->buffer, "/gfx/sprites/actions/freq-%d", freq);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

static action_t *actions_new_compass()
{
    action_t *action = calloc(1, sizeof(action_t));
    char *dirs[] = {"NorthWest", "North", "NorthEast", "West", "East", "SouthWest", "South", "SouthEast"};
    uint16_t dir = rand() % 8;
    uint16_t expected = (dir < 4 ? dir + 1 : dir + 2);

    while (expected == control_panel.left.compass)
    {
        dir = rand() % 8;
        expected = (dir < 4 ? dir + 1 : dir + 2);
    }

    action->element = ELEMENT_COMPASS;
    action->expected[0] = expected;
    sprintf(action->buffer, "/gfx/sprites/actions/compass-%s", dirs[dir]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

// actions for center station
action_t *actions_new_lights()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_PRESSURIZER;
    strcpy(action->buffer, "/gfx/sprites/actions/lights-%d.sprite");

    return action;
}

static action_t *actions_new_press()
{
    action_t *action = calloc(1, sizeof(action_t));
    uint16_t press[] = {1000, 2000, 3000, 4000};
    uint8_t pres = rand() % 4;

    while (1 + pres == control_panel.pressure)
        pres = rand() % 4;

    action->element = ELEMENT_PRESSURIZER;
    action->expected[0] = 1 + pres;
    sprintf(action->buffer, "/gfx/sprites/actions/press-%d", press[pres]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

static action_t *actions_new_rod()
{
    action_t *action = calloc(1, sizeof(action_t));
    char *colors[] = {"red", "green", "blue", "orange"};
    char letters[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    char numbers[] = {'1', '2', '3', '4'};
    uint8_t color = rand() % 4;
    while (color == color_prev)
        color = rand() % 4;

    uint8_t pos_x = rand() % 6;
    uint8_t pos_y = rand() % 4;
    while (pos_x == pos_x_prev || pos_y == pos_y_prev || control_panel.center.grid[pos_y][pos_x] == color + 1)
    {
        pos_x = rand() % 6;
        pos_y = rand() % 4;
    }

    action->element = ELEMENT_GRID;
    action->expected[0] = 1 + color;
    color_prev = action->expected[0];
    action->expected[1] = pos_y;
    pos_y_prev = pos_y;
    action->expected[2] = pos_x;
    pos_x_prev = pos_x;
    sprintf(action->buffer, "/gfx/sprites/actions/rod-%s-%c%c", colors[color], letters[pos_x], numbers[pos_y]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

// actions for right station
static action_t *actions_new_power()
{
    action_t *action = calloc(1, sizeof(action_t));
    uint16_t powers[] = {0, 125, 250, 375, 500};
    uint8_t power = rand() % 5;

    while (powers[power] == control_panel.power)
        power = rand() % 5;

    action->element = ELEMENT_TURBINES;
    action->expected[0] = powers[power];
    sprintf(action->buffer, "/gfx/sprites/actions/power-%d", powers[power]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

static action_t *actions_new_pumps()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_PUMPS;
    action->expected[0] = 9;
    strcpy(action->buffer, "/gfx/sprites/actions/pumps-%d.sprite");

    return action;
}

static action_t *actions_new_keypad()
{
    static const struct
    {
        uint8_t expected[8];
        const char *buffer;
    } calls[] = {
        {{4, 6, 7, 6, 4, 7, 2, 9}, "/gfx/sprites/actions/call-incident-%d.sprite"},
        {{5, 7, 3, 3, 4, 7, 3, 8}, "/gfx/sprites/actions/call-nuclear-%d.sprite"},
        {{2, 9, 8, 2, 2, 9, 4, 1}, "/gfx/sprites/actions/call-president-%d.sprite"},
        {{1, 0, 2, 9, 4, 8, 5, 9}, "/gfx/sprites/actions/call-public-%d.sprite"},
        {{3, 9, 0, 5, 1, 2, 7, 3}, "/gfx/sprites/actions/call-spare-%d.sprite"},
        {{3, 6, 4, 8, 3, 9, 5, 7}, "/gfx/sprites/actions/call-treasury-%d.sprite"},
    };
    uint16_t name = rand() % 6;

    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_KEYPAD;
    for (uint8_t i = 0; i < 8; i++)
        action->expected[i] = calls[name].expected[i];
    strcpy(action->buffer, calls[name].buffer);

    return action;
}

static action_t *actions_new_az5()
{
    action_t *action = calloc(1, sizeof(action_t));
    char *dependants[] = {"turbines", "compass"};
    element_t elements[] = {ELEMENT_TURBINES, ELEMENT_COMPASS};
    uint8_t desired_values[] = {0, 2};

    uint8_t dependants_pos = rand() % 2;
    action->element = ELEMENT_AZ5;
    action->expected[0] = elements[dependants_pos];
    action->expected[1] = desired_values[dependants_pos];

    sprintf(action->buffer, "/gfx/sprites/actions/az5-%s", dependants[dependants_pos]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

static action_new actions_new[] = {
    actions_new_power,
    actions_new_compass,
    actions_new_press,
    actions_new_rod,
    actions_new_pumps,
    actions_new_freq,
    actions_new_keypad,
    actions_new_az5,
};

uint16_t actions_get_points()
{
    return points;
}

uint8_t difficulty()
{
    if (points < NORMAL)
        return ELEMENT_PUMPS;

    if (points < HARD)
        return ELEMENT_KEYPAD;

    return ELEMENT_TUTORIAL;
}

static action_t *get_action(uint8_t unwanted_action1, uint8_t unwanted_action2)
{
    uint8_t element = difficulty();
    uint8_t wanted_action = rand() % element;
    while (wanted_action == unwanted_action1 || wanted_action == unwanted_action2)
        wanted_action = rand() % element;
    return actions_new[wanted_action]();
}

action_pair_t actions_get_current()
{
    return pair;
}

bool actions_next()
{
    if (pair.top != 0)
    {
        dfs_free_sprites(pair.top->text);
        free(pair.top);
    }

    pair.top = get_action(255, 255);
    pair.bottom = NULL;
    if (points > HARD && pair.top->element != ELEMENT_AZ5)
    {
        if (pair.bottom != 0)
        {
            dfs_free_sprites(pair.bottom->text);
            free(pair.bottom);
        }
        pair.bottom = get_action(pair.top->element, ELEMENT_AZ5);
    }
    points++;
    return (points >= HARD * 2);
}

void actions_reset()
{
    points = 0;
    actions_next();
    points = 0;
}

// Tutorial
#define ACTIONS_TUTORIAL_COUNT 14

static uint8_t current_element = 0;
static action_t *actions_tutorial[ACTIONS_TUTORIAL_COUNT];

static const struct
{
    element_t element;
    uint16_t expected[3];
    const char *path;
    uint8_t show;
} actions_tutorial_specs[ACTIONS_TUTORIAL_COUNT] = {
    {ELEMENT_TUTORIAL, {0}, "/gfx/sprites/actions/tuto-welcome.sprite", SHOW_NONE},
    {ELEMENT_TUTORIAL, {0}, "/gfx/sprites/actions/tuto-intro.sprite", SHOW_NONE},
    {ELEMENT_TUTORIAL, {0}, "/gfx/sprites/actions/tuto-center.sprite", SHOW_STATION},
    {ELEMENT_PRESSURIZER, {2}, "/gfx/sprites/actions/tuto-press-2000.sprite", SHOW_STATION},
    {ELEMENT_GRID, {1, 3, 3}, "/gfx/sprites/actions/tuto-rod-red-D4.sprite", SHOW_STATION},
    {ELEMENT_COMPASS, {9}, "/gfx/sprites/actions/tuto-compass-SouthEast.sprite", SHOW_STATION},
    {ELEMENT_RADIO, {360}, "/gfx/sprites/actions/tuto-freq-360.sprite", SHOW_STATION},
    {ELEMENT_AZ5, {ELEMENT_COMPASS, 2}, "/gfx/sprites/actions/tuto-az5-compass.sprite", SHOW_STATION},
    {ELEMENT_PUMPS, {9}, "/gfx/sprites/actions/tuto-pumps.sprite", SHOW_STATION},
    {ELEMENT_TURBINES, {250}, "/gfx/sprites/actions/tuto-power-250.sprite", SHOW_STATION},
    {ELEMENT_TURBINES, {125}, "/gfx/sprites/actions/tuto-power-125.sprite", SHOW_STATION},
    {ELEMENT_KEYPAD, {1, 2, 3}, "/gfx/sprites/actions/tuto-call-123.sprite", SHOW_STATION},
    {ELEMENT_TUTORIAL, {0}, "/gfx/sprites/actions/tuto-geiger.sprite", SHOW_DANGER},
    {ELEMENT_TUTORIAL, {0, 1}, "/gfx/sprites/actions/tuto-luck.sprite", SHOW_DANGER},
};

static action_t *actions_new_tutorial(uint8_t i)
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = actions_tutorial_specs[i].element;
    action->expected[0] = actions_tutorial_specs[i].expected[0];
    action->expected[1] = actions_tutorial_specs[i].expected[1];
    action->expected[2] = actions_tutorial_specs[i].expected[2];
    action->text2 = dfs_load_sprite(actions_tutorial_specs[i].path);
    action->show = actions_tutorial_specs[i].show;

    return action;
}

action_t *actions_new_lights_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_PRESSURIZER;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-lights.sprite");

    return action;
}
action_t *actions_get_current_tutorial()
{
    return actions_tutorial[current_element - 1];
}

bool actions_next_tutorial()
{

    if (current_element >= ACTIONS_TUTORIAL_COUNT)
        return true;
    current_element++;
    return false;
}

void actions_reset_tutorial()
{
    if (actions_tutorial[0] == NULL)
    {
        for (uint8_t i = 0; i < ACTIONS_TUTORIAL_COUNT; i++)
            actions_tutorial[i] = actions_new_tutorial(i);
    }

    current_element = 0;
    actions_next_tutorial();
}
