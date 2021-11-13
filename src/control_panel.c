#include "control_panel.h"

#include <string.h>
#include <stdlib.h>

#include "colors.h"
#include "debug.h"
#include "dfs.h"
#include "graphics.h"
#include "rdp.h"
#include "sfx.h"

control_panel_t control_panel;
static sprite_t *tiles[816] = {0};

extern uint32_t __width;
extern uint32_t __height;

extern uint32_t colors[];

static void instruments_draw(display_context_t disp)
{
    uint8_t x = 200;
    uint8_t y = 0;
    uint8_t width = __width - x;
    uint8_t height = 140;

    rdp_draw_filled_rectangle_size(x, y, width, height, colors[COLOR_BORDER]);
    rdp_draw_filled_rectangle_size(x + 2, y, width - 2, height - 2, colors[COLOR_PANEL]);

    graphics_draw_textf_with_background(disp, x + 8, y + 8, colors[COLOR_BROWN], "INSTRUMENTS");

    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x + 44, y + 30, colors[COLOR_BLACK], (control_panel.temp < 0 ? "%.2dC" : " %.2dC"), control_panel.temp);
    graphics_set_color(colors[COLOR_WHITE], 0);
    graphics_draw_text(disp, x + 46, y + 46, "TEMP");

    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x + 44, y + 30 + 37, colors[COLOR_BLACK], "%.4dW", control_panel.power);
    graphics_set_color(colors[COLOR_WHITE], 0);
    graphics_draw_text(disp, x + 46, y + 46 + 37, "POWER");

    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x + 44, y + 30 + 37 + 37, colors[COLOR_BLACK], "%02dHz", control_panel.freq);
    graphics_set_color(colors[COLOR_WHITE], 0);
    graphics_draw_text(disp, x + 46 + 4, y + 46 + 37 + 37, "FREQ");

    uint8_t gauge_height = 90;
    rdp_draw_filled_rectangle_size(x + 20, 30, 10, gauge_height + 4 + 4, colors[COLOR_BLACK]);

    rdp_draw_filled_rectangle_size(x + 20 + 4, 34 + gauge_height - control_panel.stress * gauge_height / 100, 2, control_panel.stress * gauge_height / 100, colors[COLOR_RED]);
    graphics_draw_text(disp, x + 10, 56, "S\nT\nR\nE\nS\nS");
}

static void instructions_draw(display_context_t disp)
{
    uint8_t x = 200;
    uint8_t y = 138;
    uint8_t width = __width - x;
    uint8_t height = __height - y;

    rdp_draw_filled_rectangle_size(x, y, width, height, colors[COLOR_BORDER]);
    rdp_draw_filled_rectangle_size(x + 2, y + 2, width - 2, height - 2, colors[COLOR_PANEL]);

    graphics_draw_textf_with_background(disp, x + 8, y + 8, colors[COLOR_BROWN], "INSTRUCTIONS");

    rdp_draw_filled_rectangle_absolute(x + 8, y + 28, __width - 8, __height - 10, colors[COLOR_BLACK]);

    graphics_set_color(colors[COLOR_YELLOW], 0);
    graphics_draw_text(disp, x + 8 + 4, y + 28 + 4, actions_get_current()->text);
    graphics_set_color(colors[COLOR_WHITE], 0);
}
void control_panel_draw(display_context_t disp)
{

    instruments_draw(disp);

    rdp_draw_filled_rectangle_absolute(0, 160, 200, __height, colors[COLOR_BORDER]);
    rdp_draw_filled_rectangle_absolute(0, 162, __width - 120, __height, colors[COLOR_PANEL]);
    switch (control_panel.current_station)
    {
    case 0:
        station_left_draw(disp);
        break;
    case 1:
        station_center_draw(disp);
        break;
    case 2:
        station_right_draw(disp);
        break;
    }

    instructions_draw(disp);

    if (control_panel.stress < 50)
        rdp_draw_sprite_with_texture(tiles[12], 162, 30, 0);
    else
        rdp_draw_sprite_with_texture(tiles[(control_panel.stress % 2 == 0 ? 12 : 1)], 162, 30, 0);
}

void control_panel_input(input_t *input)
{
    switch (control_panel.current_station)
    {
    case 0:
        station_left_input(input);
        break;
    case 1:
        station_center_input(input);
        break;
    case 2:
        station_right_input(input);
        break;
    }
}

control_panel_status_t control_panel_check_status(action_t *action)
{
    if (control_panel.stress == 100)
        return DEAD;

    for (int i = 0; i < action->num_buttons; ++i)
    {
        switch (action->buttons[i].station)
        {
        case STATION_LEFT:
            break;
        case STATION_CENTER:
            switch (action->buttons[i].label)
            {
            case LABEL_A:
                if (control_panel.center.A != action->buttons[i].expected)
                    return INCORRECT;
                break;
            case LABEL_B:
                if (control_panel.center.B != action->buttons[i].expected)
                    return INCORRECT;
                break;
            }
            break;
        case STATION_RIGHT:
            switch (action->buttons[i].label)
            {
            case LABEL_A:
                if (control_panel.right.A != action->buttons[i].expected)
                    return INCORRECT;
                break;
            case LABEL_B:
                if (control_panel.right.B != action->buttons[i].expected)
                    return INCORRECT;
                break;
            }
            break;
        }
    }

    if (control_panel.stress < 10)
        control_panel.stress = 0;
    else
        control_panel.stress -= 10;

    return CORRECT;
}

void control_panel_init()
{
    for (int i = 0; i < 816; i++)
        tiles[i] = dfs_load_spritef("/gfx/sprites/stations/tile_%04d.sprite", i);

    control_panel_reset();
}

void control_panel_reset()
{
    memset(&control_panel, 0, sizeof(control_panel));
    control_panel.stress = 0;
    control_panel.current_station = 1;
}

void control_panel_timer()
{
    control_panel.stress++;
    if (control_panel.stress <= STRESS_THRESHOLD)
        sfx_set_next_music(SFX_IDLE);
    else if (control_panel.stress <= HELL_THRESHOLD)
        sfx_set_next_music(SFX_STRESS);
    else
        sfx_set_next_music(SFX_HELL);
    control_panel.freq = 10 + (rand() % 230);
    control_panel.power = 1000 + (rand() % 7777);
    control_panel.temp = (rand() % 100) - 40;
    if (control_panel.freq < 100)
        control_panel.freq = -control_panel.freq;
}

void station_left_draw(display_context_t disp)
{
    graphics_draw_textf_with_background(disp, 8, 168, colors[COLOR_BROWN], "COMMUNICATIONS");

    uint16_t x = 16;
    uint16_t y = 210;

    for (int i = 0; i < 10; i++)
        rdp_draw_sprite_with_texture(tiles[51 + i], x + i * 18, y, 0);
}

void station_left_input(input_t *input)
{
    if (input->R)
        control_panel.current_station++;

    station_left_t *station = &(control_panel.left);

    if (input->up)
        station->DPAD[INPUT_UP] = !station->DPAD[INPUT_UP];
    if (input->down)
        station->DPAD[INPUT_DOWN] = !station->DPAD[INPUT_DOWN];
    if (input->left)
        station->DPAD[INPUT_LEFT] = !station->DPAD[INPUT_LEFT];
    if (input->right)
        station->DPAD[INPUT_RIGHT] = !station->DPAD[INPUT_RIGHT];

    if (input->Z)
        station->Z = !station->Z;
}

void station_center_draw(display_context_t disp)
{
    graphics_draw_textf_with_background(disp, 8, 168, colors[COLOR_BROWN], "FANS CONTROLS");

    uint16_t x = 32;
    uint16_t y = 210;

    station_center_t *station = &(control_panel.center);

    rdp_draw_sprite_with_texture(tiles[(station->A ? 448 : 447)], x, y, 0);
    rdp_draw_sprite_with_texture(tiles[(station->B ? 482 : 481)], x + 20, y, 0);

    x = 160;
    y = 200;
    rdp_draw_sprite_with_texture(tiles[(station->C[0] ? 550 : 549)], x, y - 12, 0);
    rdp_draw_sprite_with_texture(tiles[(station->C[1] ? 550 : 549)], x - 12, y, 0);
    rdp_draw_sprite_with_texture(tiles[(station->C[1] ? 550 : 549)], x + 12, y, 0);
    rdp_draw_sprite_with_texture(tiles[(station->C[3] ? 550 : 549)], x, y + 12, 0);
}

void station_center_input(input_t *input)
{
    if (input->L)
        control_panel.current_station--;
    else if (input->R)
        control_panel.current_station++;

    station_center_t *station = &(control_panel.center);

    if (input->up)
        station->DPAD[INPUT_UP] = !station->DPAD[INPUT_UP];
    if (input->down)
        station->DPAD[INPUT_DOWN] = !station->DPAD[INPUT_DOWN];
    if (input->left)
        station->DPAD[INPUT_LEFT] = !station->DPAD[INPUT_LEFT];
    if (input->right)
        station->DPAD[INPUT_RIGHT] = !station->DPAD[INPUT_RIGHT];

    if (input->C_up)
        station->C[INPUT_UP] = !station->C[INPUT_UP];
    if (input->C_down)
        station->C[INPUT_DOWN] = !station->C[INPUT_DOWN];
    if (input->C_left)
        station->C[INPUT_LEFT] = !station->C[INPUT_LEFT];
    if (input->C_right)
        station->C[INPUT_RIGHT] = !station->C[INPUT_RIGHT];

    if (input->A)
    {
        station->A = !station->A;
        sfx_play(CH_SFX, SFX_BUTTON, false);
    }
    if (input->B)
        station->B = !station->B;
}

void station_right_draw(display_context_t disp)
{
    graphics_draw_textf_with_background(disp, 18, 168, colors[COLOR_BROWN], "TURBINE CONTROLS");

    uint16_t x = 32;
    uint16_t y = 210;

    station_right_t *station = &(control_panel.right);

    rdp_draw_sprite_with_texture(tiles[(station->A ? 444 : 446)], x, y, 0);
    rdp_draw_sprite_with_texture(tiles[(station->B ? 478 : 480)], x + 20, y, 0);
}

void station_right_input(input_t *input)
{
    if (input->L)
        control_panel.current_station--;

    station_right_t *station = &(control_panel.right);

    if (input->C_up)
        station->C[0] = !station->C[0];
    if (input->C_down)
        station->C[1] = !station->C[1];
    if (input->C_left)
        station->C[2] = !station->C[2];
    if (input->C_right)
        station->C[3] = !station->C[3];

    if (input->A)
        station->A = !station->A;
    if (input->B)
        station->B = !station->B;
    if (input->Z)
        station->Z = !station->Z;
    if (input->R)
        station->R = !station->R;
}