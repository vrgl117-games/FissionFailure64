#include "actions.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "dfs.h"

static action_t *actions[8];
static uint8_t current = 0;

static action_t *actions_new_freq()
{
    action_t *action = calloc(1, sizeof(action_t));
    // uint16_t freqs[] = {100, 101, 107, 108, 110, 111, 114, 115, 117, 118, 124, 125, 126, 127, 131, 132, 133, 134, 139, 140, 141, 143, 144, 146, 147, 148, 150, 151, 153, 156, 157, 158, 159, 160, 160, 163, 164, 165, 166, 167, 170, 172, 173, 173, 174, 176, 177, 177, 179, 180, 180, 181, 183, 184, 186, 187, 189, 190, 190, 191, 193, 193, 194, 196, 197, 197, 198, 200, 203, 204, 205, 206, 207, 210, 210, 211, 212, 213, 214, 217, 218, 219, 220, 222, 223, 224, 225, 226, 227, 229, 230, 231, 232, 235, 236, 237, 238, 239, 239, 242, 243, 244, 245, 246, 249, 251, 252, 252, 253, 255, 256, 256, 258, 259, 259, 260, 262, 263, 265, 266, 268, 269, 269, 270, 272, 272, 273, 275, 276, 276, 277, 279, 282, 283, 284, 285, 286, 289, 289, 290, 291, 292, 293, 296, 297, 298, 299, 301, 302, 303, 304, 305, 306, 308, 309, 310, 311, 314, 315, 316, 317, 318, 318, 321, 322, 323, 324, 325, 328, 330, 331, 331, 332, 334, 335, 335, 337, 338, 338, 339, 341, 342, 344, 345, 347, 348, 348, 349, 351, 351, 352, 354, 355, 355, 356, 358, 361, 362, 363, 364, 365, 368, 368, 369, 370, 371, 372, 375, 377, 378, 380, 381, 382, 384, 385, 387, 388, 389, 394, 395, 396, 397, 401, 402, 403, 404, 410, 411, 413, 414, 417, 418, 420, 421, 427, 428, 430, 434, 435, 437, 444, 447, 451, 454, 461, 468, 60, 67, 74, 77, 81, 84, 91, 93, 94, 98};
    //uint16_t freq = rand() % 256;
    uint16_t freqs[] = {180, 404};
    uint16_t freq = rand() % 2;
    action->station = STATION_LEFT;
    action->element = ELEMENT_RADIO;
    action->expected[0] = freqs[freq];
    action->text = dfs_load_sprites_freq("/gfx/sprites/actions/freq-%d-%d.sprite", freqs[freq]);

    return action;
}

static action_t *actions_new_spare_part()
{
    action_t *action = calloc(1, sizeof(action_t));

    action->station = STATION_CENTER;
    action->element = ELEMENT_GRID;
    action->expected[0] = 3;
    action->expected[1] = 9;
    action->expected[2] = 0;
    action->expected[3] = 5;
    action->expected[4] = 1;
    action->expected[5] = 2;
    action->expected[6] = 7;
    action->expected[7] = 3;
    action->text = dfs_load_sprites("/gfx/sprites/actions/call-spare-%d.sprite");

    return action;
}

static action_t *actions_new_rod()
{
    action_t *action = calloc(1, sizeof(action_t));
    char *colors[] = {"red", "blue", "green", "orange"};
    uint8_t color = rand() % 4;
    char pos_x[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    char pos_y[] = {'1', '2', '3', '4'};
    uint8_t po_x = rand() % 6;
    uint8_t po_y = rand() % 4;

    action->station = STATION_CENTER;
    action->element = ELEMENT_GRID;
    action->expected[0] = color;
    action->expected[1] = po_y;
    action->expected[2] = po_x;
    action->text = dfs_load_sprites_rod("/gfx/sprites/actions/rod_%s_%c%c-%d.sprite", colors[color], pos_x[po_x], pos_y[po_y]);

    return action;
}

static action_t *actions_new_power(bool off)
{
    action_t *action = calloc(1, sizeof(action_t));

    action->station = STATION_CENTER;
    action->element = ELEMENT_B;
    action->expected[0] = off;

    action->text = dfs_load_sprites((off ? "/gfx/sprites/actions/power_off-%d.sprite" : "/gfx/sprites/actions/power_on-%d.sprite"));

    return action;
}

void actions_init()
{
    actions_reset();
}

action_pair_t actions_get_current()
{
    action_pair_t pair = {.top = actions[current]};
    if (current > 0)
        pair.bottom = actions[current + 1];
    return pair;
}

bool actions_next()
{
    if (current > 3)
        current += 2;
    else
        current++;
    return (current >= 8);
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
    actions[0] = actions_new_rod();
    actions[1] = actions_new_freq();
    actions[2] = actions_new_rod();
    actions[3] = actions_new_spare_part();
    actions[4] = actions_new_rod();
    actions[5] = actions_new_power(true);
    actions[6] = actions_new_power(false);
    actions[7] = actions_new_rod();
}