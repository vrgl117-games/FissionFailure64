#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

#include "input.h"

typedef enum screen
{
    credits,
    intro,
    game,
    game_over,
    message,
    options,
    pause,
    title,
    //   vru,
    win,
} screen_t;

typedef enum pause_selection
{
    pause_none = -1,
    pause_resume = 0,
    pause_options = 1,
    pause_credits = 2,
    pause_quit = 3,
} pause_selection_t;

bool screen_credits(display_context_t disp, input_t *input);
bool screen_intro(display_context_t disp);
screen_t screen_game(display_context_t disp, input_t *input);
bool screen_game_over(display_context_t disp, input_t *input);
void screen_load_title_resources();
bool screen_message_draw(display_context_t disp);
void screen_message_load();
void screen_message_unload();
bool screen_options(display_context_t disp, input_t *input);
pause_selection_t screen_pause(display_context_t disp, input_t *input, bool reset);
void screen_timer();
bool screen_title_draw(display_context_t disp, input_t *input);
void screen_title_load();
void screen_title_unload();
//bool screen_vru(display_context_t disp, input_t *input);
bool screen_win(display_context_t disp, input_t *input);

#endif //__SCREENS_H__
