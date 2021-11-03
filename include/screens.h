#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

#include "input.h"

typedef enum screen
{
    intro,
    game,
    game_over,
    vru,
    win,
} screen_t;

bool screen_intro(display_context_t disp);
screen_t screen_game(display_context_t disp, input_t *input);
bool screen_game_over(display_context_t disp, input_t *input);
void screen_timer();
bool screen_vru(display_context_t disp, input_t *input);
bool screen_win(display_context_t disp, input_t *input);

#endif //__SCREENS_H__
