#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

#include "input.h"

typedef enum screen
{
    intro,
    title,
    game,
    game_over,
    message,
    //   vru,
    win,
} screen_t;

bool screen_intro(display_context_t disp);
screen_t screen_game(display_context_t disp, input_t *input);
bool screen_game_over(display_context_t disp, input_t *input);
void screen_load_title_resources();
bool screen_message_draw(display_context_t disp);
void screen_message_load();
void screen_message_unload();
void screen_timer();
bool screen_title_draw(display_context_t disp, input_t *input);
void screen_title_load();
void screen_title_unload();
//bool screen_vru(display_context_t disp, input_t *input);
bool screen_win(display_context_t disp, input_t *input);

#endif //__SCREENS_H__
