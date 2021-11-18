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

    for (uint8_t i = 0; i < 3; i++)
    {
        uint8_t x = rand() % GRID_SIZE;
        uint8_t y = rand() % GRID_SIZE;
        if (control_panel.center.grid[y][x] != 0)
        {
            i--;
            continue;
        }
        control_panel.center.grid[y][x] = i + 1;
    }

    for (uint8_t i = 0; i < NUM_SLIDERS; i++)
        control_panel.left.sliders[i] = 1 + rand() % 3;

    control_panel.freq = 200 + (100 * control_panel.left.sliders[0]) + (-33 * control_panel.left.sliders[1]) + (17 * control_panel.left.sliders[2]);
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
    control_panel.power = 100 + (rand() % 777);
    control_panel.temp = (rand() % 100) - 40;
}

void station_left_draw(display_context_t disp)
{
    uint16_t x = 16;
    uint16_t y = 152;

    station_left_t *station = &(control_panel.left);

    //rdp_draw_filled_rectangle_size(x, y - 20, 124, 14, colors[COLOR_BG]);
    //rdp_draw_filled_rectangle_size(x + 2, y - 18, 120, 10, colors[COLOR_WHITE]);
    //for (uint8_t i = 0; i < 11; i++)
    //rdp_draw_filled_rectangle_size(x + 11 + i * 10, y - (i % 2 ? 16 : 14), 1, (i % 2 ? 6 : 4), colors[COLOR_BLACK]);

    if (!control_panel.lights_off)
        graphics_draw_textf_with_background(disp, x, y - 20, colors[COLOR_BROWN], "RADIO");

    rdp_draw_filled_rectangle_size(x + 50, y - 20, 74, 14, colors[COLOR_BG]);
    rdp_draw_filled_rectangle_size(x + 52, y - 18, 70, 10, colors[COLOR_WHITE]);
    for (uint8_t i = 0; i < 6; i++)
        rdp_draw_filled_rectangle_size(x + 61 + i * 10, y - (i % 2 ? 16 : 14), 1, (i % 2 ? 6 : 4), colors[COLOR_BLACK]);

    //rdp_draw_filled_rectangle_size(x + 2 + control_panel.freq * 12 / 100, y - 17, 1, 8, colors[COLOR_ORANGE]);

    rdp_draw_filled_rectangle_size(x + 52 + control_panel.freq * 12 / 100, y - 17, 1, 8, colors[COLOR_ORANGE]);
    if (!control_panel.lights_off)
    {
        for (uint8_t i = 0; i < NUM_SLIDERS; i++)
        {
            rdp_draw_filled_rectangle_size(x, y + 10 + 18 * i, 124, 2, colors[COLOR_BLACK]);
            rdp_draw_filled_rectangle_size(x + station->sliders[i] * 25, (y + 18 * i) + 4, 24, 12, (i == station->selected_slider) ? colors[COLOR_YELLOW] : colors_dark[i + 1]);
            rdp_draw_filled_rectangle_size(x + 2 + station->sliders[i] * 25, (y + 18 * i) + 6, 20, 8, colors[i + 1]);
            rdp_draw_filled_rectangle_size(x + 5 + station->sliders[i] * 25, (y + 18 * i) + 8, 2, 4, colors_dark[i + 1]);
            rdp_draw_filled_rectangle_size(x + 11 + station->sliders[i] * 25, (y + 18 * i) + 8, 2, 4, colors_dark[i + 1]);
            rdp_draw_filled_rectangle_size(x + 17 + station->sliders[i] * 25, (y + 18 * i) + 8, 2, 4, colors_dark[i + 1]);
        }
    }

    x = x + 160;
    y = 152;

    if (!control_panel.lights_off)
    {
        graphics_draw_textf_with_background(disp, x - 20, y - 20, colors[COLOR_BROWN], "LIGHTS");

        rdp_draw_filled_rectangle_size(x, y, 32, 16, colors[COLOR_BG]);
    }
    if (!station->button_z)
    {
        rdp_draw_filled_rectangle_size(x + 2, y + 2, 12, 12, colors_dark[COLOR_GREEN]);
        rdp_draw_filled_rectangle_size(x + 4, y + 4, 8, 8, colors[COLOR_GREEN]);
        rdp_draw_filled_rectangle_size(x + 7, y + 7, 2, 2, colors_dark[COLOR_GREEN]);
    }
    else
    {
        rdp_draw_filled_rectangle_size(x + 16 + 2, y + 2, 12, 12, colors_dark[COLOR_RED]);
        rdp_draw_filled_rectangle_size(x + 16 + 4, y + 4, 8, 8, colors[COLOR_RED]);
        rdp_draw_filled_rectangle_size(x + 16 + 7, y + 7, 2, 2, colors_dark[COLOR_RED]);
    }

    x = x + 8;
    y = y + 42;

    if (!control_panel.lights_off)
    {
        graphics_draw_textf_with_background(disp, x - 22, y, colors[COLOR_BROWN], "PUMPS");

        rdp_draw_filled_rectangle_size(16, 214, 190, 12, colors[COLOR_BG]);
        rdp_draw_filled_rectangle_size(18, 216, 186, 8, colors[COLOR_WHITE]);
    }
    uint8_t width = 2 + (station->rotations * 20);
    rdp_draw_filled_rectangle_size(20, 218, width, 4, colors[COLOR_RED]);
}

void station_left_input(input_t *input)
{
    if (input->R)
        control_panel.current_station++;

    station_left_t *station = &(control_panel.left);

    if (input->up)
        if (station->selected_slider > 0)
            station->selected_slider--;
    if (input->down)
        if (station->selected_slider < NUM_SLIDERS - 1)
            station->selected_slider++;
    if (input->left)
        if (station->sliders[station->selected_slider] > 0)
            station->sliders[station->selected_slider]--;
    if (input->right)
        if (station->sliders[station->selected_slider] < SLIDER_POSITONS - 1)
            station->sliders[station->selected_slider]++;

    control_panel.freq = 200 + (100 * station->sliders[0]) + (-33 * station->sliders[1]) + (17 * station->sliders[2]);

    if (input->Z)
    {
        station->button_z = !station->button_z;
        control_panel.lights_off = station->button_z;
    }

    uint8_t joystick = 0;
    if (input->y > 90)
    {
        if (input->x < -90)
            joystick = 1;
        else if (input->x > 90)
            joystick = 3;
        else
            joystick = 2;
    }
    else if (input->y < -90)
    {
        if (input->x < -90)
            joystick = 7;
        else if (input->x > 90)
            joystick = 5;
        else
            joystick = 6;
    }
    else
    {
        if (input->x < -90)
            joystick = 8;
        else if (input->x > 90)
            joystick = 4;
    }

    if (joystick > station->joystick)
    {
        if (joystick == 8)
        {
            joystick = 0;
            if (station->rotations != 9)
                station->rotations++;
        }
        station->joystick = joystick;
    }
    else if (joystick != 0 && station->rotations != 9)
    {
        station->joystick = 0;
        station->rotations = 0;
    }
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
        if (station->gridselector_y < GRID_SIZE - 1)
            station->gridselector_y++;
    if (input->left)
        if (station->gridselector_x > 0)
            station->gridselector_x--;
    if (input->right)
        if (station->gridselector_x < GRID_SIZE - 1)
            station->gridselector_x++;

    if (input->B)
        station->button_b = !station->button_b;

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
        if (input->C_down && station->gridselector_y < GRID_SIZE - 1 && (station->grid[station->gridselector_y + 1][station->gridselector_x] == 0))
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
        if (input->C_right && station->gridselector_x < GRID_SIZE - 1 && (station->grid[station->gridselector_y][station->gridselector_x + 1] == 0))
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