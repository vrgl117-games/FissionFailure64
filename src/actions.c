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
    uint16_t freqs[] = {0, 10, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 15, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 20, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 25, 250, 255, 260, 265, 270, 275, 280, 285, 290, 295, 30, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 35, 350, 355, 360, 365, 370, 375, 380, 385, 390, 395, 40, 400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 45, 450, 455, 460, 465, 470, 475, 480, 485, 490, 495, 5, 50, 500, 505, 510, 515, 520, 525, 530, 535, 540, 545, 55, 550, 555, 560, 565, 570, 575, 580, 585, 590, 595, 60, 600, 605, 610, 615, 620, 65, 70, 75, 80, 85, 90, 95};
    uint16_t freq = rand() % 125;

    while (freqs[freq] == control_panel.freq)
        freq = rand() % 88;

    action->element = ELEMENT_RADIO;
    action->expected[0] = freqs[freq];
    sprintf(action->buffer, "/gfx/sprites/actions/freq-%d", freqs[freq]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

static action_t *actions_new_compass()
{
    action_t *action = calloc(1, sizeof(action_t));
    char *dirs[] = {"NorthWest", "North", "NorthEast", "West", "East", "SouthWest", "South", "SouthEast"};
    uint16_t dir = rand() % 8;

    while (dir == control_panel.left.compass)
        dir = rand() % 8;

    action->element = ELEMENT_COMPASS;
    action->expected[0] = (dir < 4 ? dir + 1 : dir + 2);
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
    while (pos_x == pos_x_prev)
        pos_x = rand() % 6;
    uint8_t pos_y = rand() % 4;
    while (pos_y == pos_y_prev)
        pos_y = rand() % 4;

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

    while (power == control_panel.power)
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
    uint16_t name = rand() % 6;

    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_KEYPAD;
    switch (name)
    {
    case 0:
        action->expected[0] = 4;
        action->expected[1] = 6;
        action->expected[2] = 7;
        action->expected[3] = 6;
        action->expected[4] = 4;
        action->expected[5] = 7;
        action->expected[6] = 2;
        action->expected[7] = 9;
        strcpy(action->buffer, "/gfx/sprites/actions/call-incident-%d.sprite");
        break;
    case 1:
        action->expected[0] = 5;
        action->expected[1] = 7;
        action->expected[2] = 3;
        action->expected[3] = 3;
        action->expected[4] = 4;
        action->expected[5] = 7;
        action->expected[6] = 3;
        action->expected[7] = 8;
        strcpy(action->buffer, "/gfx/sprites/actions/call-nuclear-%d.sprite");
        break;
    case 2:
        action->expected[0] = 2;
        action->expected[1] = 9;
        action->expected[2] = 8;
        action->expected[3] = 2;
        action->expected[4] = 2;
        action->expected[5] = 9;
        action->expected[6] = 4;
        action->expected[7] = 1;
        strcpy(action->buffer, "/gfx/sprites/actions/call-president-%d.sprite");
        break;
    case 3:
        action->expected[0] = 1;
        action->expected[1] = 0;
        action->expected[2] = 2;
        action->expected[3] = 9;
        action->expected[4] = 4;
        action->expected[5] = 8;
        action->expected[6] = 5;
        action->expected[7] = 9;
        strcpy(action->buffer, "/gfx/sprites/actions/call-public-%d.sprite");
        break;
    case 4:
        action->expected[0] = 3;
        action->expected[1] = 9;
        action->expected[2] = 0;
        action->expected[3] = 5;
        action->expected[4] = 1;
        action->expected[5] = 2;
        action->expected[6] = 7;
        action->expected[7] = 3;
        strcpy(action->buffer, "/gfx/sprites/actions/call-spare-%d.sprite");
        break;
    case 5:
        action->expected[0] = 3;
        action->expected[1] = 6;
        action->expected[2] = 4;
        action->expected[3] = 8;
        action->expected[4] = 3;
        action->expected[5] = 9;
        action->expected[6] = 5;
        action->expected[7] = 7;
        strcpy(action->buffer, "/gfx/sprites/actions/call-treasury-%d.sprite");
        break;
    }

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

static action_new actions[] = {
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
    return actions[wanted_action]();
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
u_int8_t current_element = 0;

static action_t *actions_new_press_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_PRESSURIZER;
    action->expected[0] = 2;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-press-2000.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_freq_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_RADIO;
    action->expected[0] = 360;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-freq-360.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_rod_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_GRID;
    action->expected[0] = 1;
    action->expected[1] = 3;
    action->expected[2] = 3;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-rod-red-D4.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_compass_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_COMPASS;
    action->expected[0] = 9;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-compass-SouthEast.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_az5_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_AZ5;
    action->expected[0] = ELEMENT_COMPASS;
    action->expected[1] = 2;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-az5-compass.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_pumps_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_PUMPS;
    action->expected[0] = 9;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-pumps.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_power_up_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_TURBINES;
    action->expected[0] = 250;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-power-250.sprite");
    action->show = SHOW_STATION;

    return action;
}

action_t *actions_new_lights_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_PRESSURIZER;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-lights.sprite");

    return action;
}

static action_t *actions_new_power_down_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_TURBINES;
    action->expected[0] = 125;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-power-125.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_keypad_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_KEYPAD;
    action->expected[0] = 1;
    action->expected[1] = 2;
    action->expected[2] = 3;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-call-123.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_welcome_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_TUTORIAL;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-welcome.sprite");

    return action;
}

static action_t *actions_new_intro_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_TUTORIAL;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-intro.sprite");

    return action;
}

static action_t *actions_new_center_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_TUTORIAL;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-center.sprite");
    action->show = SHOW_STATION;

    return action;
}

static action_t *actions_new_geiger_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_TUTORIAL;
    action->expected[1] = 1;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-geiger.sprite");
    action->show = SHOW_DANGER;

    return action;
}

static action_t *actions_new_good_luck_tutorial()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->element = ELEMENT_TUTORIAL;
    action->expected[1] = 1;
    action->text2 = dfs_load_sprite("/gfx/sprites/actions/tuto-luck.sprite");
    action->show = SHOW_DANGER;

    return action;
}

static action_new actions_tutorial[] = {
    actions_new_welcome_tutorial,
    actions_new_intro_tutorial,
    actions_new_center_tutorial,

    actions_new_press_tutorial,
    actions_new_rod_tutorial,

    actions_new_compass_tutorial,
    actions_new_freq_tutorial,
    actions_new_az5_tutorial,

    actions_new_pumps_tutorial,
    actions_new_power_up_tutorial,
    actions_new_power_down_tutorial,
    actions_new_keypad_tutorial,

    actions_new_geiger_tutorial,
    actions_new_good_luck_tutorial,

    NULL,
};

static action_t *actual_actions[14];

action_pair_t actions_get_current_tutorial()
{
    return pair;
}

bool actions_next_tutorial()
{
    pair.top = NULL;
    pair.bottom = NULL;
    if (actions_tutorial[current_element] == NULL)
        return true;
    pair.top = actual_actions[current_element];
    current_element++;
    return false;
}

void actions_reset_tutorial()
{
    if (actual_actions[0] == NULL)
    {
        for (uint8_t i = 0; i < 14; i++)
            actual_actions[i] = actions_tutorial[i]();
    }

    current_element = 0;
    actions_next_tutorial();
}
