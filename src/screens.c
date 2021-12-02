#include "screens.h"

#include <stdlib.h>

#include "colors.h"
#include "control_panel.h"
#include "dfs.h"
#include "rdp.h"
#include "scientist.h"
#include "sfx.h"

extern uint32_t __width;
extern uint32_t __height;
extern uint32_t colors[];
extern control_panel_t control_panel;

static volatile uint32_t ticks = 0;
static volatile bool alt = false;

// credits screen
bool screen_credits(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    sprite_t *credits_sp = dfs_load_sprite("/gfx/sprites/ui/credits_big.sprite");
    graphics_draw_sprite(disp, __width / 2 - credits_sp->width / 2, 10, credits_sp);
    free(credits_sp);

    sprite_t *programming_sp = dfs_load_sprite("/gfx/sprites/ui/programming.sprite");
    graphics_draw_sprite(disp, __width / 2 - programming_sp->width / 2, 70, programming_sp);
    free(programming_sp);
    sprite_t *isabel_victor_sp = dfs_load_sprite("/gfx/sprites/ui/isabel_victor.sprite");
    graphics_draw_sprite(disp, __width / 2 - isabel_victor_sp->width / 2, 100, isabel_victor_sp);
    free(isabel_victor_sp);
    sprite_t *vrgl117games_sp = dfs_load_sprite("/gfx/sprites/ui/vrgl117games.sprite");
    graphics_draw_sprite(disp, __width / 2 - vrgl117games_sp->width / 2, 124, vrgl117games_sp);
    free(vrgl117games_sp);

    sprite_t *art_sp = dfs_load_sprite("/gfx/sprites/ui/art.sprite");
    graphics_draw_sprite(disp, 60 - art_sp->width / 2, 150, art_sp);
    free(art_sp);
    sprite_t *jphosho_sp = dfs_load_sprite("/gfx/sprites/ui/jphosho.sprite");
    graphics_draw_sprite(disp, 60 - jphosho_sp->width / 2, 180, jphosho_sp);
    free(jphosho_sp);
    sprite_t *atjphosho_sp = dfs_load_sprite("/gfx/sprites/ui/atjphosho.sprite");
    graphics_draw_sprite(disp, 60 - atjphosho_sp->width / 2, 204, atjphosho_sp);
    free(atjphosho_sp);

    sprite_t *music_sp = dfs_load_sprite("/gfx/sprites/ui/music.sprite");
    graphics_draw_sprite(disp, __width - 80 - music_sp->width / 2, 150, music_sp);
    free(music_sp);
    sprite_t *manuhoz_sp = dfs_load_sprite("/gfx/sprites/ui/manuhoz.sprite");
    graphics_draw_sprite(disp, __width - 80 - manuhoz_sp->width / 2, 180, manuhoz_sp);
    free(manuhoz_sp);
    sprite_t *radiatorhymn_sp = dfs_load_sprite("/gfx/sprites/ui/radiatorhymn.sprite");
    graphics_draw_sprite(disp, __width - 80 - radiatorhymn_sp->width / 2, 204, radiatorhymn_sp);
    free(radiatorhymn_sp);

    return (input->A || input->start);
}

// display the n64 logo and then the vrgl117 games logo.
// return true when the animation is done.
bool screen_intro(display_context_t disp)
{
    static int anim = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_DARK]);

    rdp_detach_display();
    sprite_t *intro = NULL;

    switch (anim)
    {
    case 1 ... 9:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", anim);
        break;
    case 10 ... 50:
        intro = dfs_load_sprite("/gfx/sprites/intro/n64brew_jam_logo.sprite");
        break;
    case 51 ... 60:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", 60 - anim);
        break;
    case 61 ... 69:
        intro = dfs_load_spritef("/gfx/sprites/intro/vrgl117_logo_%d.sprite", anim - 60);
        break;
    case 70 ... 100:
        intro = dfs_load_sprite("/gfx/sprites/intro/vrgl117_logo.sprite");
        break;
    case 101 ... 109:
        intro = dfs_load_spritef("/gfx/sprites/intro/vrgl117_logo_%d.sprite", 110 - anim);
        break;
    }

    if (intro != NULL)
    {
        graphics_draw_sprite(disp, __width / 2 - intro->width / 2, __height / 2 - intro->height / 2, intro);
        free(intro);
    }

    anim++;
    return (anim >= 112);
}
static sprite_t *window_idle_sp = NULL;
static sprite_t *window_stressed_sp = NULL;
static sprite_t *window_hell_sp = NULL;
static sprite_t *window_hell_alt_sp = NULL;
void screen_game_load()
{
    window_idle_sp = dfs_load_sprite("/gfx/sprites/window/idle.sprite");
    window_stressed_sp = dfs_load_sprite("/gfx/sprites/window/stressed.sprite");
    window_hell_sp = dfs_load_sprite("/gfx/sprites/window/hell.sprite");
    window_hell_alt_sp = dfs_load_sprite("/gfx/sprites/window/hell_alt.sprite");
}

void screen_game_unload()
{
    free(window_idle_sp);
    free(window_stressed_sp);
    free(window_hell_sp);
    free(window_hell_alt_sp);
}

// main screen for the game
screen_t screen_game(display_context_t disp, input_t *input)
{

    control_panel_input(input);

    switch (control_panel_check_status(actions_get_current()))
    {
    case DEAD:
        return game_over;

    case CORRECT:
        if (actions_next())
            return win;
        break;
    case INCORRECT:
        break;
    }

    sprite_t *window = window_idle_sp;
    switch (control_panel.mode)
    {
    case STRESSED:
        window = window_stressed_sp;
        break;
    case HELL:
        window = (alt ? window_hell_alt_sp : window_hell_sp);
        break;
    default:
        break;
    }

    rdp_attach(disp);

    if (!control_panel.lights_off)
    {
        rdp_draw_filled_fullscreen(colors[COLOR_PANEL]);
        rdp_draw_filled_rectangle_size(40, 0, 180, 120, colors[COLOR_BG]);
    }
    else
        rdp_draw_filled_fullscreen(colors[COLOR_DARK]);

    graphics_draw_sprite(disp, 70, 20, window);

    scientist_draw();

    control_panel_draw(disp);

    return game;
}

// game over screen
bool screen_game_over(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    sprite_t *boom_sp = dfs_load_sprite("/gfx/sprites/window/boom.sprite");
    graphics_draw_sprite_trans(disp, __width / 2 - boom_sp->width / 2, __height / 2 - boom_sp->height / 2, boom_sp);
    free(boom_sp);

    sprite_t *gameover_sp = dfs_load_sprite("/gfx/sprites/ui/gameover.sprite");
    graphics_draw_sprite(disp, __width / 2 - gameover_sp->width / 2, 20, gameover_sp);
    free(gameover_sp);

    sprite_t *continue_sp = dfs_load_sprite("/gfx/sprites/ui/continue.sprite");
    graphics_draw_sprite(disp, __width / 2 - continue_sp->width / 2, 200, continue_sp);
    free(continue_sp);

    return (input->A || input->start);
}

static sprite_t *message_sp = NULL;
void screen_message_load()
{
    message_sp = dfs_load_sprite("/gfx/sprites/ui/message.sprite");
}
void screen_message_unload()
{
    free(message_sp);
}

bool screen_message_draw(display_context_t disp, input_t *input)
{
    static int anim = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    graphics_draw_sprite(disp, __width / 2 - message_sp->width / 2, 60, message_sp);

    anim++;
    return (input->A || input->start || anim >= 102);
}

// pause menu
screen_selection_t screen_pause(display_context_t disp, input_t *input, bool reset)
{
    static uint8_t selected = 0;

    if (reset)
        selected = 0;

    if (input->up)
        selected = (selected == 0 ? 4 : selected - 1);
    else if (input->down)
        selected = (selected == 4 ? 0 : selected + 1);

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    sprite_t *pause_sp = dfs_load_sprite("/gfx/sprites/ui/pause_big.sprite");
    graphics_draw_sprite(disp, __width / 2 - pause_sp->width / 2, 10, pause_sp);
    free(pause_sp);

    sprite_t *resume_sp = dfs_load_sprite((selected == 0 ? "/gfx/sprites/ui/resume_selected.sprite" : "/gfx/sprites/ui/resume.sprite"));
    graphics_draw_sprite(disp, __width / 2 - resume_sp->width / 2, 90, resume_sp);
    free(resume_sp);
    sprite_t *phonebook_sp = dfs_load_sprite((selected == 1 ? "/gfx/sprites/ui/phonebook_selected.sprite" : "/gfx/sprites/ui/phonebook.sprite"));
    graphics_draw_sprite(disp, __width / 2 - phonebook_sp->width / 2, 115, phonebook_sp);
    free(phonebook_sp);
    sprite_t *tutorial_sp = dfs_load_sprite((selected == 2 ? "/gfx/sprites/ui/tutorial_selected.sprite" : "/gfx/sprites/ui/tutorial.sprite"));
    graphics_draw_sprite(disp, __width / 2 - tutorial_sp->width / 2, 140, tutorial_sp);
    free(tutorial_sp);
    sprite_t *credits_sp = dfs_load_sprite((selected == 3 ? "/gfx/sprites/ui/credits_selected.sprite" : "/gfx/sprites/ui/credits.sprite"));
    graphics_draw_sprite(disp, __width / 2 - credits_sp->width / 2, 165, credits_sp);
    free(credits_sp);
    sprite_t *quit_sp = dfs_load_sprite((selected == 4 ? "/gfx/sprites/ui/quit_selected.sprite" : "/gfx/sprites/ui/quit.sprite"));
    graphics_draw_sprite(disp, __width / 2 - quit_sp->width / 2, 190, quit_sp);
    free(quit_sp);

    if (input->A)
        return selected;
    if (input->start)
        return screen_selection_resume;
    return screen_selection_none;
}

//phonebook menu
bool screen_phonebook(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_WHITE]);

    rdp_detach_display();

    sprite_t *sp = dfs_load_sprite("/gfx/sprites/phonebook.sprite");
    graphics_draw_sprite(disp, __width / 2 - sp->width / 2, __height / 2 - sp->height / 2, sp);
    free(sp);

    return (input->A || input->start);
}

static sprite_t *logo_sp = NULL;
static sprite_t *start_sp = NULL;
static sprite_t *start_selected_sp = NULL;
static sprite_t *tutorial_sp = NULL;
static sprite_t *tutorial_selected_sp = NULL;
void screen_title_load()
{
    logo_sp = dfs_load_sprite("/gfx/sprites/ui/logo.sprite");
    start_sp = dfs_load_sprite("/gfx/sprites/ui/caps_start.sprite");
    start_selected_sp = dfs_load_sprite("/gfx/sprites/ui/caps_start_selected.sprite");
    tutorial_sp = dfs_load_sprite("/gfx/sprites/ui/caps_tutorial.sprite");
    tutorial_selected_sp = dfs_load_sprite("/gfx/sprites/ui/caps_tutorial_selected.sprite");
}

void screen_title_unload()
{
    free(logo_sp);
    free(start_sp);
    free(start_selected_sp);
    free(tutorial_sp);
    free(tutorial_selected_sp);
}

screen_selection_t screen_title_draw(display_context_t disp, input_t *input)
{
    static uint8_t selected = screen_selection_resume;

    if (input->up)
        selected = (selected == screen_selection_resume ? screen_selection_tutorial : screen_selection_resume);
    else if (input->down)
        selected = (selected == screen_selection_tutorial ? screen_selection_resume : screen_selection_tutorial);

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    graphics_draw_sprite(disp, __width / 2 - logo_sp->width / 2, 20, logo_sp);

    if (selected == screen_selection_tutorial)
        graphics_draw_sprite(disp, __width / 2 - start_sp->width / 2, 160, start_sp);
    else if (ticks % 40 > 19)
        graphics_draw_sprite(disp, __width / 2 - start_sp->width / 2, 160, start_selected_sp);

    if (selected == screen_selection_resume)
        graphics_draw_sprite(disp, __width / 2 - tutorial_sp->width / 2, 190, tutorial_sp);
    else if (ticks % 40 > 19)
        graphics_draw_sprite(disp, __width / 2 - tutorial_sp->width / 2, 190, tutorial_selected_sp);

    if (input->A || input->start)
        return selected;
    return screen_selection_none;
}

// tutorial screen
bool screen_tutorial(display_context_t disp, input_t *input, bool reset)
{
    static uint8_t page = 0;

    if (reset)
        page = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    sprite_t *sp;
    switch (page)
    {
    case 0:
        sp = dfs_load_sprite("/gfx/sprites/ui/how_to.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 10, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/tutorial/left.sprite");
        graphics_draw_sprite(disp, 20, 60, sp);
        free(sp);
        sp = dfs_load_sprite("/gfx/sprites/ui/left.sprite");
        graphics_draw_sprite(disp, 40, 140, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/tutorial/center.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 60, sp);
        free(sp);
        sp = dfs_load_sprite("/gfx/sprites/ui/center.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 140, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/tutorial/right.sprite");
        graphics_draw_sprite(disp, __width - 20 - sp->width, 60, sp);
        free(sp);
        sp = dfs_load_sprite("/gfx/sprites/ui/right.sprite");
        graphics_draw_sprite(disp, __width - 40 - sp->width, 140, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/ui/use_l_r.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 170, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/ui/continue.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 200, sp);
        free(sp);
        if (input->start)
            return true;
        if (input->A || input->R)
            page++;
        if (input->L)
            page--;
        break;
    case 1:
        sp = dfs_load_sprite("/gfx/sprites/ui/how_to_left.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 10, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/ui/tuto_left.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, __height / 2 - sp->height / 2, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/ui/continue.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 200, sp);
        free(sp);
        if (input->start)
            return true;
        if (input->A || input->R)
            page++;
        if (input->L)
            page--;
        break;
    case 2:
        sp = dfs_load_sprite("/gfx/sprites/ui/how_to_center.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 10, sp);
        free(sp);
        sp = dfs_load_sprite("/gfx/sprites/ui/tuto_center.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, __height / 2 - sp->height / 2, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/ui/continue.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 200, sp);
        free(sp);
        if (input->start)
            return true;
        if (input->A || input->R)
            page++;
        if (input->L)
            page--;
        break;
    case 3:
        sp = dfs_load_sprite("/gfx/sprites/ui/how_to_right.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 10, sp);
        free(sp);
        sp = dfs_load_sprite("/gfx/sprites/ui/tuto_right.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, __height / 2 - sp->height / 2, sp);
        free(sp);

        sp = dfs_load_sprite("/gfx/sprites/ui/continue.sprite");
        graphics_draw_sprite(disp, __width / 2 - sp->width / 2, 200, sp);
        free(sp);
        if (input->start)
            return true;
        if (input->A || input->R)
            page++;
        if (input->L)
            page--;
        break;
    }

    return (page == 4);
}

// end game screen
bool screen_win(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    sprite_t *win_sp = dfs_load_sprite("/gfx/sprites/ui/win.sprite");
    graphics_draw_sprite(disp, __width / 2 - win_sp->width / 2, 20, win_sp);
    free(win_sp);

    sprite_t *continue_sp = dfs_load_sprite("/gfx/sprites/ui/continue.sprite");
    graphics_draw_sprite(disp, __width / 2 - continue_sp->width / 2, 200, continue_sp);
    free(continue_sp);

    return (input->A || input->start);
}

void screen_timer()
{
    ticks++;

    if (ticks % 10 == 0)
        alt = (rand() % 100 >= 80);
}
