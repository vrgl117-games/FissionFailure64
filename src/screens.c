#include <stdlib.h>

#include "colors.h"
#include "control_panel.h"
#include "dfs.h"
#include "rdp.h"
#include "screens.h"

extern uint32_t __width;
extern uint32_t __height;
extern uint32_t colors[];

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

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    sprite_t *bg = dfs_load_sprite("/gfx/sprites/bg/bg.sprite");
    graphics_draw_sprite(disp, 0, 0, bg);
    free(bg);

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