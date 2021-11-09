#include "screens.h"

#include <stdlib.h>

#include "colors.h"
#include "control_panel.h"
#include "dfs.h"
#include "rdp.h"

extern uint32_t __width;
extern uint32_t __height;
extern uint32_t colors[];

static volatile uint16_t xx = 0;
static volatile bool direction = true;
static volatile uint32_t ticks = 0;

// display the n64 logo and then the vrgl117 games logo.
// return true when the animation is done.
bool screen_intro(display_context_t disp)
{
    static int anim = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();
    sprite_t *intro = NULL;

    switch (anim)
    {
    case 1 ... 9:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", anim);
        break;
    case 10 ... 30:
        intro = dfs_load_sprite("/gfx/sprites/intro/n64brew_jam_logo.sprite");
        break;
    case 31 ... 40:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", 40 - anim);
        break;
    case 41 ... 49:
        intro = dfs_load_spritef("/gfx/sprites/intro/vrgl117_logo_%d.sprite", anim - 40);
        break;
    case 50 ... 70:
        intro = dfs_load_sprite("/gfx/sprites/intro/vrgl117_logo.sprite");
        break;
    case 71 ... 79:
        intro = dfs_load_spritef("/gfx/sprites/intro/vrgl117_logo_%d.sprite", 80 - anim);
        break;
    }

    if (intro != NULL)
    {
        graphics_draw_sprite(disp, __width / 2 - intro->width / 2, __height / 2 - intro->height / 2, intro);
        free(intro);
    }

    anim++;
    return (anim >= 82);
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

    rdp_attach(disp);

    rdp_draw_filled_rectangle_size(0, 0, 210, 180, colors[COLOR_BG]);

    sprite_t *bg = dfs_load_sprite("/gfx/sprites/bg/window.sprite");
    graphics_draw_sprite(disp, 45, 20, bg);
    free(bg);

    rdp_draw_filled_rectangle_size(0, 120, 210, 2, colors[COLOR_BLACK]);

    sprite_t *scientist = dfs_load_sprite((direction ? "/gfx/sprites/scientist/right.sprite" : "/gfx/sprites/scientist/left.sprite"));
    graphics_draw_sprite_trans(disp, xx, 100, scientist);
    free(scientist);

    control_panel_draw(disp);

    rdp_detach_display();
    return game;
}

// game over screen
bool screen_game_over(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    rdp_detach_display();

    graphics_draw_text(disp, 100, 100, "GAME OVER");

    graphics_draw_text(disp, 200, 200, "<continue>");

    return (input->A || input->start);
}

static sprite_t *message_sp = NULL;
void screen_message_load()
{
    message_sp = dfs_load_sprite("/gfx/sprites/misc/message.sprite");
}
void screen_message_unload()
{
    free(message_sp);
}

bool screen_message_draw(display_context_t disp)
{
    static int anim = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    graphics_draw_sprite(disp, __width / 2 - message_sp->width / 2, 60, message_sp);

    anim++;
    return (anim >= 82);
}

static sprite_t *logo_sp = NULL;
static sprite_t *press_start_sp = NULL;
void screen_title_load()
{
    logo_sp = dfs_load_sprite("/gfx/sprites/misc/logo.sprite");
    press_start_sp = dfs_load_spritef("/gfx/sprites/misc/press_start.sprite", 12);
}

void screen_title_unload()
{
    free(logo_sp);
    free(press_start_sp);
}

bool screen_title_draw(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    if (ticks % 40 > 19)
        rdp_draw_sprite_with_texture(press_start_sp, __width / 2 - press_start_sp->width / 2, 200, 0);

    rdp_detach_display();

    graphics_draw_sprite(disp, __width / 2 - logo_sp->width / 2, 20, logo_sp);

    return (input->start);
}

/*
bool screen_vru(display_context_t disp, input_t *input)
{
    if (identify_accessory(3) == ACCESSORY_VRU)
    {
        uint8_t out[64];
        uint8_t in[64];

        // Initial sequence
        out[0] = 0x00;
        out[1] = 0x00;
        execute_raw_command(3, VRU_READ_STATUS, 2, 3, out, in);
        debugf("%02X%02X %02X%02X%02X\n", out[0], out[1], in[0], in[1], in[2]);
    }
    return true;
}
*/

// end game screen
bool screen_win(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    rdp_detach_display();

    graphics_draw_text(disp, 100, 100, "WELL DONE");

    graphics_draw_text(disp, 200, 200, "<continue>");

    return (input->A || input->start);
}

void screen_timer()
{
    ticks++;

    if (direction)
        xx++;
    else
        xx--;

    if (xx > 200)
        direction = !direction;
    if (xx <= 0)
        direction = !direction;
}
