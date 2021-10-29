#include "control_panel.h"

#include <string.h>
#include <stdlib.h>

#include "colors.h"
#include "debug.h"
#include "dfs.h"
#include "graphics.h"
#include "rdp.h"

control_panel_t control_panel;
static sprite_t *tiles[816] = {0};

extern uint32_t __width;
extern uint32_t __height;

extern uint32_t colors[];

static void instruments_draw(display_context_t disp)
{
    rdp_draw_filled_rectangle_with_border_size(211, 0, 108, 140, colors[COLOR_PANEL], colors[COLOR_BORDER]);

    graphics_draw_textf_with_background(disp, __width - 105, 4, colors[COLOR_BROWN], "INSTRUMENTS");

    uint32_t x = __width - 60;
    uint32_t y = 26;

    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x, y, colors[COLOR_BLACK], (control_panel.temp < 0 ? "%.2dC" : " %.2dC"), control_panel.temp);
    graphics_set_color(colors[COLOR_WHITE], 0);
    graphics_draw_text(disp, x + 2, y + 16, "TEMP");

    y = 60;
    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x, y, colors[COLOR_BLACK], "%.4dW", control_panel.power);
    graphics_set_color(colors[COLOR_WHITE], 0);
    graphics_draw_text(disp, x + 2, y + 16, "POWER");

    y = 100;
    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x, y, colors[COLOR_BLACK], "%02dHz", control_panel.freq);
    graphics_set_color(colors[COLOR_WHITE], 0);
    graphics_draw_text(disp, x + 7, y + 16, "FREQ");

    rdp_draw_filled_rectangle_size(__width - 90, 26, 10, 108, colors[COLOR_BLACK]);

    rdp_draw_filled_rectangle_size(__width - 86, 30 + 100 - control_panel.stress, 2, control_panel.stress, colors[COLOR_RED]);
    graphics_draw_text(disp, __width - 100, 56, "S\nT\nR\nE\nS\nS");
}
void control_panel_draw(display_context_t disp)
{

    instruments_draw(disp);

    rdp_draw_filled_rectangle_with_border_size(0, 180, 212, 56, colors[COLOR_PANEL], colors[COLOR_BORDER]);
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

    rdp_draw_filled_rectangle_with_border_size(211, 138, 108, 98, colors[COLOR_PANEL], colors[COLOR_BORDER]);

    graphics_draw_textf_with_background(disp, __width - 105, 142, colors[COLOR_BROWN], "INSTRUCTIONS");

    rdp_draw_filled_rectangle_size(__width - 105, 164, 100, 62, colors[COLOR_BLACK]);
    graphics_set_color(colors[COLOR_YELLOW], 0);
    graphics_draw_text(disp, __width - 102, 168, actions_get_current()->text);
    graphics_set_color(colors[COLOR_WHITE], 0);

    if (control_panel.stress < 50)
        rdp_draw_sprite_with_texture(tiles[12], 172, 20, 0);
    else
        rdp_draw_sprite_with_texture(tiles[(control_panel.stress % 2 == 0 ? 12 : 1)], 172, 20, 0);
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
    {
        tiles[i] = dfs_load_spritef("/gfx/sprites/stations/tile_%04d.sprite", i);
    }
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
    control_panel.freq = 10 + (rand() % 230);
    control_panel.power = 1000 + (rand() % 7777);
    control_panel.temp = (rand() % 100) - 40;
    if (control_panel.freq < 100)
        control_panel.freq = -control_panel.freq;
}

void station_left_draw(display_context_t disp)
{
    graphics_draw_textf_with_background(disp, 4, 184, colors[COLOR_BROWN], "COMMUNICATIONS");

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
    graphics_draw_textf_with_background(disp, 4, 184, colors[COLOR_BROWN], "FANS CONTROLS");

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
        station->A = !station->A;
    if (input->B)
        station->B = !station->B;
}

void station_right_draw(display_context_t disp)
{
    graphics_draw_textf_with_background(disp, 4, 184, colors[COLOR_BROWN], "TURBINE CONTROLS");

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