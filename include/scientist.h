
#ifndef __SCIENTIST_H__
#define __SCIENTIST_H__

#include "dfs.h"

#define MIN_X -200
#define MAX_X 420
#define MAX_VISIBLE_X 220
#define MAX_Y 12
#define NUM_SCIENTIST 7
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct scientist
{
    uint8_t mode; //0 -> idle, 1 -> stressed, 2 -> hell
    sprites_t *sprites[3];

    int16_t x;
    uint8_t y;
    uint8_t y_offset;

    bool h_direction;
    bool v_direction;
} scientist_t;

void scientist_draw();
void scientist_init();
void scientist_timer();

#endif //__SCIENTIST_H__
