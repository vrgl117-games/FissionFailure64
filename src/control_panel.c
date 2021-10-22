#include "control_panel.h"

#include <string.h>

#include "colors.h"
#include "debug.h"
#include "dfs.h"
#include "rdp.h"

control_panel_t control_panel;
static sprite_t *tiles[816] = {0};

extern uint32_t __width;
extern uint32_t __height;

extern uint32_t colors[];

void control_panel_draw(display_context_t disp)
{
    rdp_draw_filled_rectangle_size(0, (__height / 3) * 2, __width, __height / 2, colors[COLOR_BLACK]);
    rdp_draw_filled_rectangle_size(2, (__height / 3) * 2 + 2, __width - 4, __height / 2 - 4, colors[COLOR_BG]);

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

    rdp_draw_filled_rectangle_size(4, (__height / 3) * 2 + 4, (__width - 8) * control_panel.stress / 100, 2, colors[COLOR_RED]);

    graphics_draw_text(disp, 4, 110, actions_get_current()->text);
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
    if (control_panel.stress == 0)
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

    control_panel.stress += 10;
    if (control_panel.stress > 100)
        control_panel.stress = 100;

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
    control_panel.stress = 100;
    control_panel.current_station = 1;
}

void control_panel_timer()
{
    control_panel.stress--;
}

void station_left_draw(display_context_t disp)
{
    rdp_draw_sprite_with_texture(tiles[60], 70 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[51], 90 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[52], 110 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[53], 130 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[54], 150 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[55], 170 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[56], 190 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[57], 210 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[58], 230 - 8, (__height / 3) * 2 + 30, 0);
    rdp_draw_sprite_with_texture(tiles[59], 250 - 8, (__height / 3) * 2 + 30, 0);

    graphics_draw_text(disp, 4, (__height / 3) * 2 + 10, "LEFT STATION");
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
    station_center_t *station = &(control_panel.center);

    rdp_draw_sprite_with_texture(tiles[(station->A ? 448 : 447)], 160, (__height / 3) * 2 + 30, 0);

    rdp_draw_sprite_with_texture(tiles[(station->B ? 482 : 481)], 180, (__height / 3) * 2 + 40, 0);

    rdp_draw_sprite_with_texture(tiles[(station->C[0] ? 550 : 549)], 260, (__height / 3) * 2 + 10, 0);
    rdp_draw_sprite_with_texture(tiles[(station->C[1] ? 550 : 549)], 260 - 15, (__height / 3) * 2 + 25, 0);
    rdp_draw_sprite_with_texture(tiles[(station->C[1] ? 550 : 549)], 260 + 15, (__height / 3) * 2 + 25, 0);
    rdp_draw_sprite_with_texture(tiles[(station->C[3] ? 550 : 549)], 260, (__height / 3) * 2 + 40, 0);

    graphics_draw_text(disp, 4, (__height / 3) * 2 + 10, "CENTER STATION");
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
    station_right_t *station = &(control_panel.right);

    rdp_draw_sprite_with_texture(tiles[(station->A ? 444 : 446)], 180, (__height / 3) * 2 + 20, 0);

    rdp_draw_sprite_with_texture(tiles[(station->B ? 478 : 480)], 180, (__height / 3) * 2 + 40, 0);

    graphics_draw_text(disp, 4, (__height / 3) * 2 + 10, "RIGHT STATION");
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