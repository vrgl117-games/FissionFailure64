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
    case 31 ... 39:
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

bool screen_message(display_context_t disp)
{
    static int anim = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    sprite_t *message = dfs_load_sprite("/gfx/sprites/misc/message.sprite");
    graphics_draw_sprite(disp, __width / 2 - message->width / 2, 60, message);
    free(message);

    anim++;
    return (anim >= 82);
}

bool screen_title(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();

    sprite_t *logo = dfs_load_sprite("/gfx/sprites/misc/logo.sprite");
    graphics_draw_sprite(disp, __width / 2 - logo->width / 2, 20, logo);
    free(logo);

    if (ticks % 40 > 19)
    {
        sprite_t *press_start = dfs_load_sprite("/gfx/sprites/misc/press_start.sprite");
        graphics_draw_sprite(disp, __width / 2 - press_start->width / 2, 200, press_start);
        free(press_start);
    }

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
