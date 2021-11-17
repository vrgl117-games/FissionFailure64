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
extern uint32_t colors_dark[];

static void danger_bar_draw(display_context_t disp)
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t width = 40;
    uint8_t height = 120;

    if (!control_panel.lights_off)
    {
        rdp_draw_filled_rectangle_size(x, y, width, height, colors[COLOR_BORDER]);
        rdp_draw_filled_rectangle_size(x, y, width - 2, height, colors[COLOR_PANEL]);
        rdp_draw_filled_rectangle_size(x + 8, 8, 10, 104, colors[COLOR_BLACK]);
        graphics_draw_text(disp, x + 24, 12, "D\nA\nN\nG\nE\nR\n\nL\nE\nV\nE\nL");
    }

    rdp_draw_filled_rectangle_size(x + 12, 10, 2, 100, colors[COLOR_WHITE]);
    rdp_draw_filled_rectangle_size(x + 12, 10 + 100 - control_panel.stress, 2, control_panel.stress, colors[(control_panel.mode == HELL ? COLOR_RED : (control_panel.mode == STRESSED ? COLOR_ORANGE : COLOR_YELLOW))]);
}

static void instruments_draw(display_context_t disp)
{
    uint8_t x = 220;
    uint8_t y = 130;
    uint8_t width = __width - x;
    uint8_t height = __height - y;

    if (!control_panel.lights_off)
    {
        rdp_draw_filled_rectangle_size(x, y, width, height, colors[COLOR_BORDER]);
        rdp_draw_filled_rectangle_size(x + 2, y, width - 2, height - 2, colors[COLOR_PANEL]);

        graphics_draw_textf_with_background(disp, x + 8, y + 8, colors[COLOR_BROWN], "STATUS");
        graphics_draw_text(disp, x + 8, y + 32, "TEMP");
        graphics_draw_text(disp, x + 8, y + 32 + 24, "POWER");
        graphics_draw_text(disp, x + 8, y + 32 + 24 + 24, "FREQ");
    }

    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x + 54, y + 30, colors[COLOR_BLACK], (control_panel.temp < 0 ? "%.2dC" : " %.2dC"), control_panel.temp);
    graphics_set_color(colors[COLOR_WHITE], 0);

    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x + 54, y + 30 + 24, colors[COLOR_BLACK], "%.03dW", control_panel.power);
    graphics_set_color(colors[COLOR_WHITE], 0);

    graphics_set_color(colors[COLOR_RED], 0);
    graphics_draw_textf_with_background(disp, x + 46, y + 30 + 24 + 24, colors[COLOR_BLACK], "%02dHz", control_panel.freq);
    graphics_set_color(colors[COLOR_WHITE], 0);
}

static void instructions_draw(display_context_t disp)
{
    uint8_t x = 220;
    uint8_t y = 0;
    uint8_t width = __width - x;
    uint8_t height = 130;

    if (!control_panel.lights_off)
    {
        rdp_draw_filled_rectangle_size(x, y, width, height, colors[COLOR_BORDER]);
        rdp_draw_filled_rectangle_size(x + 2, y, width - 2, height - 2, colors[COLOR_PANEL]);

        graphics_draw_textf_with_background(disp, x + 8, y + 8, colors[COLOR_BROWN], "COMMANDS");

        rdp_draw_filled_rectangle_size(x + 8, y + 28, width - 16, 94, colors[COLOR_BLACK]);
    }

    graphics_set_color(colors[COLOR_YELLOW], 0);
    graphics_draw_text(disp, x + 8 + 4, y + 28 + 4, actions_get_current()->text);
    graphics_set_color(colors[COLOR_WHITE], 0);
}
void control_panel_draw(display_context_t disp)
{
    danger_bar_draw(disp);
    instructions_draw(disp);

    instruments_draw(disp);

    if (!control_panel.lights_off)
    {
        rdp_draw_filled_rectangle_absolute(0, 120, 220, __height, colors[COLOR_BORDER]);
        rdp_draw_filled_rectangle_absolute(0, 122, __width - 100, __height, colors[COLOR_PANEL]);
    }
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

    if (control_panel.mode == IDLE)
        rdp_draw_sprite_with_texture(tiles[12], 198, 10, 0);
    else
        rdp_draw_sprite_with_texture(tiles[(control_panel.stress % 2 == 0 ? 1 : 12)], 198, 10, 0);
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
            case LABEL_GRID:
                if (control_panel.center.grid[action->buttons[i].expected[0]][action->buttons[i].expected[1]] == 0)
                    return INCORRECT;
                break;
            case LABEL_A:
                if (control_panel.center.button_a || control_panel.center.button_a_presses != action->buttons[i].expected[0])
                    return INCORRECT;
                break;
            case LABEL_B:
                if (control_panel.center.button_b != action->buttons[i].expected[0])
                    return INCORRECT;
                break;
            }
            break;
        case STATION_RIGHT:
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
    control_panel.current_station = 1;
    control_panel.center.grid[2][1] = 1;
    control_panel.center.grid[3][3] = 2;
}

void control_panel_timer()
{
    control_panel.stress++;
    if (control_panel.stress <= STRESS_THRESHOLD)
    {
        control_panel.mode = IDLE;
        sfx_set_next_music(SFX_IDLE);
    }
    else if (control_panel.stress <= HELL_THRESHOLD)
    {
        control_panel.mode = STRESSED;
        sfx_set_next_music(SFX_STRESS);
    }
    else
    {
        control_panel.mode = HELL;
        sfx_set_next_music(SFX_HELL);
    }
    control_panel.freq = 10 + (rand() % 230);
    control_panel.power = 100 + (rand() % 777);
    control_panel.temp = (rand() % 100) - 40;
    if (control_panel.freq < 100)
        control_panel.freq = -control_panel.freq;
}

void station_left_draw(display_context_t disp)
{
    graphics_draw_textf_with_background(disp, 8, 168, colors[COLOR_BROWN], "COMMUNICATIONS");

    uint16_t x = 16;
    uint16_t y = 200;

    for (int i = 0; i < 10; i++)
        rdp_draw_sprite_with_texture(tiles[51 + i], x + i * 17, y, 0);
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
    uint16_t x = 10;
    uint16_t y = 130;

    station_center_t *station = &(control_panel.center);

    uint8_t cell_size = 15;
    uint8_t border = 2;
    uint8_t selector_border = 2;

    if (!control_panel.lights_off)
    {
        // Button B
        graphics_draw_textf_with_background(disp, x + 90, y, colors[COLOR_BROWN], "LIGHTS");
        rdp_draw_sprite_with_texture(tiles[(station->button_b ? 448 : 447)], x + 130, y + 15, 0);

        // Button A
        graphics_draw_textf_with_background(disp, x + 100, y + 45, colors[COLOR_BROWN], "ROD TENSION");
        rdp_draw_sprite_with_texture(tiles[(station->button_a ? 482 : 481)], x + 130, y + 60, 0);
        graphics_set_color(colors[COLOR_BLACK], 0);
        graphics_draw_textf(disp, x + 138, y + 66, "%d\n", station->button_a_presses);
        graphics_set_color(colors[COLOR_WHITE], 0);

        // Grid
        graphics_draw_textf_with_background(disp, 10, 210, colors[COLOR_BROWN], "ROD CONTROLS");
        rdp_draw_filled_rectangle_size(x, y, (GRID_SIZE * cell_size) + ((GRID_SIZE + 1) * border), (GRID_SIZE * cell_size) + ((GRID_SIZE + 1) * border), colors[COLOR_BG]);
    }

    for (uint8_t yy = 0; yy < GRID_SIZE; yy++)
    {
        for (uint8_t xx = 0; xx < GRID_SIZE; xx++)
        {
            if (!control_panel.lights_off && station->gridselector_x == xx && station->gridselector_y == yy)
                rdp_draw_filled_rectangle_size(x + border + (xx * (border + cell_size)), y + border + (yy * (border + cell_size)), cell_size, cell_size, colors[COLOR_YELLOW]);
            else
                rdp_draw_filled_rectangle_size(x + border + (xx * (border + cell_size)), y + border + (yy * (border + cell_size)), cell_size, cell_size, colors_dark[station->grid[yy][xx]]);

            rdp_draw_filled_rectangle_size(x + border + selector_border + (xx * (border + cell_size)), y + border + selector_border + (yy * (border + cell_size)), cell_size - (2 * selector_border), cell_size - (2 * selector_border), colors[station->grid[yy][xx]]);
        }
    }
}

void station_center_input(input_t *input)
{
    if (input->L)
        control_panel.current_station--;
    else if (input->R)
        control_panel.current_station++;

    station_center_t *station = &(control_panel.center);

    if (input->up)
        if (station->gridselector_y > 0)
            station->gridselector_y--;
    if (input->down)
        if (station->gridselector_y < 3)
            station->gridselector_y++;
    if (input->left)
        if (station->gridselector_x > 0)
            station->gridselector_x--;
    if (input->right)
        if (station->gridselector_x < 3)
            station->gridselector_x++;

    if (input->B)
    {
        station->button_b = !station->button_b;
        control_panel.lights_off = station->button_b;
    }

    if (input_get_A_presses())
    {
        station->button_a = true;
        station->button_a_presses = input_get_A_presses();
    }
    else
    {
        station->button_a = false;
    }

    if (station->grid[station->gridselector_y][station->gridselector_x])
    {
        if (input->C_up && station->gridselector_y > 0 && (station->grid[station->gridselector_y - 1][station->gridselector_x] == 0))
        {
            station->grid[station->gridselector_y - 1][station->gridselector_x] = station->grid[station->gridselector_y][station->gridselector_x];
            station->grid[station->gridselector_y][station->gridselector_x] = 0;
            station->gridselector_y--;
        }
        if (input->C_down && station->gridselector_y < 3 && (station->grid[station->gridselector_y + 1][station->gridselector_x] == 0))
        {
            station->grid[station->gridselector_y + 1][station->gridselector_x] = station->grid[station->gridselector_y][station->gridselector_x];
            station->grid[station->gridselector_y][station->gridselector_x] = 0;
            station->gridselector_y++;
        }
        if (input->C_left && station->gridselector_x > 0 && (station->grid[station->gridselector_y][station->gridselector_x - 1] == 0))
        {
            station->grid[station->gridselector_y][station->gridselector_x - 1] = station->grid[station->gridselector_y][station->gridselector_x];
            station->grid[station->gridselector_y][station->gridselector_x] = 0;
            station->gridselector_x--;
        }
        if (input->C_right && station->gridselector_x < 3 && (station->grid[station->gridselector_y][station->gridselector_x + 1] == 0))
        {
            station->grid[station->gridselector_y][station->gridselector_x + 1] = station->grid[station->gridselector_y][station->gridselector_x];
            station->grid[station->gridselector_y][station->gridselector_x] = 0;
            station->gridselector_x++;
        }
    }
}

void station_right_draw(display_context_t disp)
{
    graphics_draw_textf_with_background(disp, 18, 168, colors[COLOR_BROWN], "TURBINE CONTROLS");

    uint16_t x = 32;
    uint16_t y = 200;

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