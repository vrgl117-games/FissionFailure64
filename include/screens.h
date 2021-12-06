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
    pause,
    phonebook,
    title,
    tutorial,
    win,
} screen_t;

typedef enum screen_selection
{
    screen_selection_none = -1,
    screen_selection_resume = 0,
    screen_selection_phonebook = 1,
    screen_selection_credits = 2,
    screen_selection_quit = 3,
    screen_selection_tutorial = 4,
} screen_selection_t;

bool screen_credits(display_context_t disp, input_t *input);
bool screen_intro(display_context_t disp);
void screen_game_load();
void screen_game_unload();
screen_t screen_game(display_context_t disp, input_t *input);
bool screen_game_over(display_context_t disp, input_t *input);
void screen_load_title_resources();
bool screen_message_draw(display_context_t disp, input_t *input);
void screen_message_load();
void screen_message_unload();
bool screen_options(display_context_t disp, input_t *input);
screen_selection_t screen_pause(display_context_t disp, input_t *input, bool reset);
bool screen_phonebook(display_context_t disp, input_t *input);
void screen_timer();
screen_selection_t screen_title_draw(display_context_t disp, input_t *input);
void screen_title_load();
void screen_title_unload();
bool screen_tutorial(display_context_t disp, input_t *input);
void screen_tutorial_load();
void screen_tutorial_unload();
bool screen_win(display_context_t disp, input_t *input);

#endif //__SCREENS_H__
