#include "actions.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "dfs.h"

static action_t *actions[11];
static uint8_t current = 0;

// actions for left station
static action_t *actions_new_freq()
{
    action_t *action = calloc(1, sizeof(action_t));
    uint16_t freqs[] = {200, 300, 400, 500, 150, 250, 350, 450, 100, 200, 300, 400, 50, 150, 250, 350, 225, 325, 425, 525, 175, 275, 375, 475, 125, 225, 325, 425, 75, 175, 275, 375, 250, 350, 450, 550, 200, 300, 400, 500, 150, 250, 350, 450, 100, 200, 300, 400, 275, 375, 475, 575, 225, 325, 425, 525, 175, 275, 375, 475, 125, 225, 325, 425, 195, 295, 395, 495, 145, 245, 345, 445, 95, 195, 295, 395, 45, 145, 245, 345, 220, 320, 420, 520, 170, 270, 370, 470, 120, 220, 320, 420, 70, 170, 270, 370, 245, 345, 445, 545, 195, 295, 395, 495, 145, 245, 345, 445, 95, 195, 295, 395, 270, 370, 470, 570, 220, 320, 420, 520, 170, 270, 370, 470, 120, 220, 320, 420, 190, 290, 390, 490, 140, 240, 340, 440, 90, 190, 290, 390, 40, 140, 240, 340, 215, 315, 415, 515, 165, 265, 365, 465, 115, 215, 315, 415, 65, 165, 265, 365, 240, 340, 440, 540, 190, 290, 390, 490, 140, 240, 340, 440, 90, 190, 290, 390, 265, 365, 465, 565, 215, 315, 415, 515, 165, 265, 365, 465, 115, 215, 315, 415, 185, 285, 385, 485, 135, 235, 335, 435, 85, 185, 285, 385, 35, 135, 235, 335, 210, 310, 410, 510, 160, 260, 360, 460, 110, 210, 310, 410, 60, 160, 260, 360, 235, 335, 435, 535, 185, 285, 385, 485, 135, 235, 335, 435, 85, 185, 285, 385, 260, 360, 460, 560, 210, 310, 410, 510, 160, 260, 360, 460, 110, 210, 310, 410};
    uint16_t freq = rand() % 88;

    action->station = STATION_LEFT;
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
    action->station = STATION_LEFT;
    action->element = ELEMENT_COMPASS;
    action->expected[0] = (dir < 4 ? dir + 1 : dir + 2);
    sprintf(action->buffer, "/gfx/sprites/actions/compass-%s", dirs[dir]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

// actions for center station
static action_t *actions_new_press()
{
    action_t *action = calloc(1, sizeof(action_t));
    uint16_t press[] = {1000, 2000, 3000, 4000};
    uint8_t pres = rand() % 4;

    action->station = STATION_CENTER;
    action->element = ELEMENT_PRESSURIZER;
    action->expected[0] = 1 + pres;
    sprintf(action->buffer, "/gfx/sprites/actions/press-%d", press[pres]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

static action_t *actions_new_lights(bool off)
{
    action_t *action = calloc(1, sizeof(action_t));

    action->station = STATION_CENTER;
    action->element = ELEMENT_LIGHTS;
    action->expected[0] = off;
    strcpy(action->buffer, (off ? "/gfx/sprites/actions/lights_off-%d.sprite" : "/gfx/sprites/actions/lights_on-%d.sprite"));

    return action;
}

static action_t *actions_new_rod(uint8_t not_color)
{
    action_t *action = calloc(1, sizeof(action_t));
    uint8_t color = (not_color + 1) % 4;
    char *colors[] = {"red", "green", "blue", "orange"};
    char pos_x[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    char pos_y[] = {'1', '2', '3', '4'};
    uint8_t po_x = rand() % 6;
    uint8_t po_y = rand() % 4;

    action->station = STATION_CENTER;
    action->element = ELEMENT_GRID;
    action->expected[0] = 1 + color;
    action->expected[1] = po_y;
    action->expected[2] = po_x;
    sprintf(action->buffer, "/gfx/sprites/actions/rod_%s_%c%c", colors[color], pos_x[po_x], pos_y[po_y]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

// actions for right station
static action_t *actions_new_power()
{
    action_t *action = calloc(1, sizeof(action_t));
    uint16_t powers[] = {0, 125, 250, 375, 500};
    uint8_t power = rand() % 5;

    action->station = STATION_RIGHT;
    action->element = ELEMENT_TURBINES;
    action->expected[0] = powers[power];
    sprintf(action->buffer, "/gfx/sprites/actions/power-%d", powers[power]);
    strcat(action->buffer, "-%d.sprite");

    return action;
}

static action_t *actions_new_pumps()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->station = STATION_RIGHT;
    action->element = ELEMENT_PUMPS;
    action->expected[0] = 9;
    strcpy(action->buffer, "/gfx/sprites/actions/pumps-%d.sprite");

    return action;
}

static action_t *actions_new_spare_part()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->station = STATION_CENTER;
    action->element = ELEMENT_KEYPAD;
    action->expected[0] = 3;
    action->expected[1] = 9;
    action->expected[2] = 0;
    action->expected[3] = 5;
    action->expected[4] = 1;
    action->expected[5] = 2;
    action->expected[6] = 7;
    action->expected[7] = 3;
    strcpy(action->buffer, "/gfx/sprites/actions/call-spare-%d.sprite");

    return action;
}

void actions_init()
{
    actions_reset();
}

action_pair_t actions_get_current()
{
    action_pair_t pair = {.top = actions[current]};
    if (current > 4 && current != 10)
        pair.bottom = actions[current + 1];
    return pair;
}

bool actions_next()
{
    if (current > 3)
        current += 2;
    else
        current++;
    return (current >= 11);
}

void actions_reset()
{
    current = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        if (actions[i])
        {
            dfs_free_sprites(actions[i]->text);
            free(actions[i]);
        }
    }
    actions[0] = actions_new_power();
    actions[1] = actions_new_freq();
    actions[2] = actions_new_compass();
    actions[3] = actions_new_spare_part();
    actions[4] = actions_new_press();
    actions[5] = actions_new_lights(true);
    actions[6] = actions_new_lights(false);
    actions[7] = actions_new_pumps();
    actions[8] = actions_new_rod(rand() % 4);
    actions[9] = actions_new_rod(actions[8]->expected[0]);
    actions[10] = actions_new_rod(actions[9]->expected[0]);
}