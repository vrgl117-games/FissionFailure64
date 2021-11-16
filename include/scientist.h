
#ifndef __SCIENTIST_H__
#define __SCIENTIST_H__

#include "dfs.h"

#define MAX_X 220
#define MAX_Y 12

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct scientist
{
    uint8_t mode; //0 -> idle, 1 -> stressed, 2 -> hell
    sprites_t *sprites[3];

    uint8_t x;
    uint8_t y;

    bool h_direction;
    bool v_direction;
} scientist_t;

void scientist_draw(uint8_t mode);
void scientist_init();
void scientist_timer();

#endif //__SCIENTIST_H__
