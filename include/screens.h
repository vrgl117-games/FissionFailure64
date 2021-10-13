#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

#include "input.h"

typedef enum screen
{
    intro,
    game,
} screen_t;

bool screen_intro(display_context_t disp);
screen_t screen_game(display_context_t disp, input_t *input);
void screen_timer_title();

#endif //__SCREENS_H__
