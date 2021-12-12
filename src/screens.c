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
    graphics_draw_sprite(disp, 100 - programming_sp->width / 2, 70, programming_sp);
    free(programming_sp);
    sprite_t *isabel_victor_sp = dfs_load_sprite("/gfx/sprites/ui/isabel_victor.sprite");
    graphics_draw_sprite(disp, 100 - isabel_victor_sp->width / 2, 100, isabel_victor_sp);
    free(isabel_victor_sp);
    sprite_t *vrgl117games_sp = dfs_load_sprite("/gfx/sprites/ui/vrgl117games.sprite");
    graphics_draw_sprite(disp, 100 - vrgl117games_sp->width / 2, 124, vrgl117games_sp);
    free(vrgl117games_sp);

    sprite_t *art_sp = dfs_load_sprite("/gfx/sprites/ui/art.sprite");
    graphics_draw_sprite(disp, __width - 80 - art_sp->width / 2, 70, art_sp);
    free(art_sp);
    sprite_t *jphosho_sp = dfs_load_sprite("/gfx/sprites/ui/jphosho.sprite");
    graphics_draw_sprite(disp, __width - 80 - jphosho_sp->width / 2, 100, jphosho_sp);
    free(jphosho_sp);
    sprite_t *atjphosho_sp = dfs_load_sprite("/gfx/sprites/ui/atjphosho.sprite");
    graphics_draw_sprite(disp, __width - 80 - atjphosho_sp->width / 2, 124, atjphosho_sp);
    free(atjphosho_sp);

    sprite_t *sfx_sp = dfs_load_sprite("/gfx/sprites/ui/sfx.sprite");
    graphics_draw_sprite(disp, 100 - sfx_sp->width / 2, 150, sfx_sp);
    free(sfx_sp);
    sprite_t *littlerobotsoundfactory_sp = dfs_load_sprite("/gfx/sprites/ui/littlerobotsoundfactory.sprite");
    graphics_draw_sprite(disp, 100 - littlerobotsoundfactory_sp->width / 2, 180, littlerobotsoundfactory_sp);
    free(littlerobotsoundfactory_sp);
    sprite_t *gowlermusic_sp = dfs_load_sprite("/gfx/sprites/ui/gowlermusic.sprite");
    graphics_draw_sprite(disp, 100 - gowlermusic_sp->width / 2, 204, gowlermusic_sp);
    free(gowlermusic_sp);

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

static sprites_t *window_idle_sp = NULL;
static sprites_t *window_stressed_sp = NULL;
static sprites_t *window_hell_sp = NULL;
static sprites_t *window_hell_alt_sp = NULL;
void screen_game_load()
{
    window_idle_sp = dfs_load_sprites("/gfx/sprites/window/idle-%d.sprite");
    window_stressed_sp = dfs_load_sprites("/gfx/sprites/window/stressed-%d.sprite");
    window_hell_sp = dfs_load_sprites("/gfx/sprites/window/hell-%d.sprite");
    window_hell_alt_sp = dfs_load_sprites("/gfx/sprites/window/hell_alt-%d.sprite");
}

void screen_game_unload()
{
    dfs_free_sprites(window_idle_sp);
    dfs_free_sprites(window_stressed_sp);
    dfs_free_sprites(window_hell_sp);
    dfs_free_sprites(window_hell_alt_sp);
}

// main screen for the game
screen_t screen_game(display_context_t disp, input_t *input)
{

    control_panel_input(input, false);

    action_pair_t current = actions_get_current();
    switch (control_panel_check_status(current))
    {
    case DEAD:
        return game_over;

    case CORRECT:
        sfx_play(CH_SFX, SFX_ACTION, false);
        if (actions_next((current.bottom ? 2 : 1)))
            return win;
        break;
    case INCORRECT:
        break;
    }

    sprites_t *window = window_idle_sp;
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

    rdp_draw_sprites_with_texture(window, 70, 20, 0);

    scientist_draw();

    control_panel_draw(disp);

    return game;
}

// game over screen
static sprites_t *boom_sp = NULL;
static sprites_t *continue_sp = NULL;
static sprites_t *gameover_sp = NULL;
void screen_game_over_load()
{
    boom_sp = dfs_load_sprites("/gfx/sprites/boom-%d.sprite");
    continue_sp = dfs_load_sprites("/gfx/sprites/ui/continue-%d.sprite");
    gameover_sp = dfs_load_sprites("/gfx/sprites/ui/gameover-%d.sprite");
}

void screen_game_over_unload()
{
    dfs_free_sprites(boom_sp);
    dfs_free_sprites(continue_sp);
    dfs_free_sprites(gameover_sp);
}

bool screen_game_over(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_draw_sprites_with_texture(boom_sp, __width / 2 - boom_sp->width / 2, __height / 2 - boom_sp->height / 2, 0);
    rdp_draw_sprites_with_texture(gameover_sp, __width / 2 - gameover_sp->width / 2, 20, 0);
    rdp_draw_sprites_with_texture(continue_sp, __width / 2 - continue_sp->width / 2, 200, 0);

    rdp_detach_display();

    return (input->A || input->start);
}

static sprite_t *message_sp = NULL;
static sprite_t *pak_sp = NULL;
static sprite_t *pak_not_sp = NULL;
static sprite_t *rumble_sp = NULL;
void screen_message_load()
{
    message_sp = dfs_load_sprite("/gfx/sprites/ui/message.sprite");
    rumble_sp = dfs_load_sprite("/gfx/sprites/ui/rumble_not_detected.sprite");
}
void screen_message_unload()
{
    free(message_sp);
    free(pak_sp);
    free(pak_not_sp);
    free(rumble_sp);
}

bool screen_message_draw(display_context_t disp, input_t *input)
{
    static uint8_t anim = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    if (identify_accessory(0) != ACCESSORY_RUMBLEPAK)
    {
        graphics_draw_sprite(disp, __width / 2 - message_sp->width / 2, 40, message_sp);
        graphics_draw_sprite(disp, __width / 2 - rumble_sp->width / 2, 120, rumble_sp);
    }
    else
        graphics_draw_sprite(disp, __width / 2 - message_sp->width / 2, __height / 2 - message_sp->height / 2, message_sp);

    anim++;
    return (anim > 128 || input->A || input->start);
}

// pause menu
screen_selection_t screen_pause(display_context_t disp, input_t *input, bool reset)
{
    static uint8_t selected = 0;

    if (reset)
        selected = 0;

    if (input->up)
        selected = (selected == 0 ? 3 : selected - 1);
    else if (input->down)
        selected = (selected == 3 ? 0 : selected + 1);

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
    sprite_t *credits_sp = dfs_load_sprite((selected == 2 ? "/gfx/sprites/ui/credits_selected.sprite" : "/gfx/sprites/ui/credits.sprite"));
    graphics_draw_sprite(disp, __width / 2 - credits_sp->width / 2, 140, credits_sp);
    free(credits_sp);
    sprite_t *quit_sp = dfs_load_sprite((selected == 3 ? "/gfx/sprites/ui/quit_selected.sprite" : "/gfx/sprites/ui/quit.sprite"));
    graphics_draw_sprite(disp, __width / 2 - quit_sp->width / 2, 165, quit_sp);
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

static sprites_t *logo_sp = NULL;
static sprites_t *start_sp = NULL;
static sprites_t *start_selected_sp = NULL;
static sprites_t *tutorial_sp = NULL;
static sprites_t *tutorial_selected_sp = NULL;
static sprite_t *nuclear_sp = NULL;
void screen_title_load()
{
    logo_sp = dfs_load_sprites("/gfx/sprites/ui/logo-%d.sprite");
    start_sp = dfs_load_sprites("/gfx/sprites/ui/caps_start-%d.sprite");
    start_selected_sp = dfs_load_sprites("/gfx/sprites/ui/caps_start_selected-%d.sprite");
    tutorial_sp = dfs_load_sprites("/gfx/sprites/ui/caps_tutorial-%d.sprite");
    tutorial_selected_sp = dfs_load_sprites("/gfx/sprites/ui/caps_tutorial_selected-%d.sprite");
    nuclear_sp = dfs_load_sprite("/gfx/sprites/ui/nuclear.sprite");
}

void screen_title_unload()
{
    dfs_free_sprites(logo_sp);
    dfs_free_sprites(start_sp);
    dfs_free_sprites(start_selected_sp);
    dfs_free_sprites(tutorial_sp);
    dfs_free_sprites(tutorial_selected_sp);
    free(nuclear_sp);
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
    if (ticks % 40 > 19)
    {
        rdp_draw_sprite_with_texture(nuclear_sp, 100, (selected == screen_selection_resume ? 162 : 192), 0);
        rdp_draw_sprite_with_texture(nuclear_sp, __width - 100 - nuclear_sp->width, (selected == screen_selection_resume ? 162 : 192), 0);
    }

    rdp_draw_sprites_with_texture(logo_sp, __width / 2 - logo_sp->width / 2, 20, 0);

    if (selected == screen_selection_tutorial)
        rdp_draw_sprites_with_texture(start_sp, __width / 2 - start_sp->width / 2, 160, 0);
    else if (ticks % 40 > 19)
        rdp_draw_sprites_with_texture(start_selected_sp, __width / 2 - start_sp->width / 2, 160, 0);

    if (selected == screen_selection_resume)
        rdp_draw_sprites_with_texture(tutorial_sp, __width / 2 - tutorial_sp->width / 2, 190, 0);
    else if (ticks % 40 > 19)
        rdp_draw_sprites_with_texture(tutorial_selected_sp, __width / 2 - tutorial_sp->width / 2, 190, 0);

    rdp_detach_display();

    if (input->A || input->start)
        return selected;
    return screen_selection_none;
}

// tutorial screen
static sprites_t *tuto_left = NULL;
static sprites_t *tuto_center = NULL;
static sprites_t *tuto_right = NULL;
void screen_tutorial_load()
{
    tuto_left = dfs_load_sprites("/gfx/sprites/tutorial/left-%d.sprite");
    tuto_center = dfs_load_sprites("/gfx/sprites/tutorial/center-%d.sprite");
    tuto_right = dfs_load_sprites("/gfx/sprites/tutorial/right-%d.sprite");
}

void screen_tutorial_unload()
{
    dfs_free_sprites(tuto_left);
    dfs_free_sprites(tuto_center);
    dfs_free_sprites(tuto_right);
}

bool screen_tutorial(display_context_t disp, input_t *input)
{

    control_panel_input(input, true);

    action_pair_t current = actions_get_current_tutorial();
    if (control_panel_check_status(current) == CORRECT)
    {
        if (current.top->element != ELEMENT_TUTORIAL)
            sfx_play(CH_SFX, SFX_ACTION, false);
        if (actions_next_tutorial())
            return true;
    }

    rdp_attach(disp);

    if (!control_panel.lights_off)
        rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);
    else
        rdp_draw_filled_fullscreen(colors[COLOR_DARK]);

    switch (current.top->element)
    {
    case ELEMENT_RADIO:
    case ELEMENT_AZ5:
    case ELEMENT_COMPASS:
        rdp_draw_sprites_with_texture(tuto_left, __width / 2 - tuto_left->width / 2, 46, 0);
        break;
    case ELEMENT_PRESSURIZER:
    case ELEMENT_GRID:
        rdp_draw_sprites_with_texture(tuto_center, __width / 2 - tuto_center->width / 2, 46, 0);
        break;
    case ELEMENT_KEYPAD:
    case ELEMENT_PUMPS:
    case ELEMENT_TURBINES:
        rdp_draw_sprites_with_texture(tuto_right, __width / 2 - tuto_right->width / 2, 46, 0);
    default:
        break;
    }

    control_panel_draw_tutorial(disp);

    return false;
}

// end game screen
static sprites_t *win_sp = NULL;
void screen_win_load()
{
    continue_sp = dfs_load_sprites("/gfx/sprites/ui/continue-%d.sprite");
    win_sp = dfs_load_sprites("/gfx/sprites/ui/win-%d.sprite");
}

void screen_win_unload()
{
    dfs_free_sprites(continue_sp);
    dfs_free_sprites(win_sp);
}
bool screen_win(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_draw_sprites_with_texture(win_sp, __width / 2 - win_sp->width / 2, 20, 0);
    rdp_draw_sprites_with_texture(continue_sp, __width / 2 - continue_sp->width / 2, 200, 0);

    scientist_win();

    rdp_detach_display();

    return (input->A || input->start);
}

void screen_timer()
{
    ticks++;

    if (ticks % 10 == 0)
        alt = (rand() % 100 >= 80);
}
