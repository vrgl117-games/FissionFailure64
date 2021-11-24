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
static sprite_t *labels[LABEL_IDX] = {0};

extern uint32_t __width;
extern uint32_t __height;

extern uint32_t colors[];
extern uint32_t colors_dark[];

static void danger_bar_draw()
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

        rdp_draw_sprite_with_texture(labels[LABEL_DANGER], x + 24, 8, 0);
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

        rdp_draw_sprite_with_texture(labels[LABEL_STATUS], x + 8, y + 8, 0);

        rdp_detach_display();

        graphics_draw_text(disp, x + 8, y + 32, "TEMP");
        graphics_draw_text(disp, x + 8, y + 32 + 24, "POWER");
        graphics_draw_text(disp, x + 8, y + 32 + 24 + 24, "FREQ");
    }
    else
        rdp_detach_display();

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

static void instructions_draw()
{
    uint8_t x = 220;
    uint8_t y = 0;
    uint8_t width = __width - x;
    uint8_t height = 130;

    if (!control_panel.lights_off)
    {
        rdp_draw_filled_rectangle_size(x, y, width, height, colors[COLOR_BORDER]);
        rdp_draw_filled_rectangle_size(x + 2, y, width - 2, height - 2, colors[COLOR_PANEL]);

        rdp_draw_sprite_with_texture(labels[LABEL_INSTRUCTIONS], x + 8, y + 8, 0);

        rdp_draw_filled_rectangle_size(x + 8, y + 28, width - 16, 94, colors[COLOR_BLACK]);
    }

    rdp_draw_sprites_with_texture(actions_get_current()->text, x + 8 + 4, y + 28 + 4, 0);
}

void control_panel_draw(display_context_t disp)
{
    danger_bar_draw();
    instructions_draw();

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

    if (control_panel.current_station == 2)
        station_right_draw_graphics(disp);
    instruments_draw(disp);
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
                if (control_panel.center.grid[action->buttons[i].expected[1]][action->buttons[i].expected[2]] != action->buttons[i].expected[0])
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

    labels[LABEL_LIGHTS] = dfs_load_sprite("/gfx/sprites/ui/label_lights.sprite");
    labels[LABEL_INSTRUCTIONS] = dfs_load_sprite("/gfx/sprites/ui/label_instructions.sprite");
    labels[LABEL_STATUS] = dfs_load_sprite("/gfx/sprites/ui/label_status.sprite");
    labels[LABEL_RADIO] = dfs_load_sprite("/gfx/sprites/ui/label_radio.sprite");
    labels[LABEL_TURBINES] = dfs_load_sprite("/gfx/sprites/ui/label_turbines.sprite");
    labels[LABEL_PUMPS] = dfs_load_sprite("/gfx/sprites/ui/label_pumps.sprite");
    labels[LABEL_CONTROL_RODS] = dfs_load_sprite("/gfx/sprites/ui/label_control_rods.sprite");
    labels[LABEL_AZ_5] = dfs_load_sprite("/gfx/sprites/ui/label_az_5.sprite");
    labels[LABEL_PRESSURIZER] = dfs_load_sprite("/gfx/sprites/ui/label_pressurizer.sprite");
    labels[LABEL_DANGER] = dfs_load_sprite("/gfx/sprites/ui/label_danger.sprite");

    control_panel_reset();
}

void control_panel_reset()
{
    memset(&control_panel, 0, sizeof(control_panel));

    // select center station
    control_panel.current_station = 1;

    // reset sliders
    for (uint8_t i = 0; i < NUM_SLIDERS; i++)
        control_panel.left.sliders[i] = 1 + rand() % 3;
    control_panel.freq = 200 + (100 * control_panel.left.sliders[0]) + (-33 * control_panel.left.sliders[1]) + (17 * control_panel.left.sliders[2]);

    // reset grid
    for (uint8_t i = 0; i < GRID_SIZE - 1; i++)
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

    // reset keypad
    control_panel.right.keypad[0][0] = 1;
    control_panel.right.keypad[0][1] = 2;
    control_panel.right.keypad[0][2] = 3;
    control_panel.right.keypad[1][0] = 4;
    control_panel.right.keypad[1][1] = 5;
    control_panel.right.keypad[1][2] = 6;
    control_panel.right.keypad[2][0] = 7;
    control_panel.right.keypad[2][1] = 8;
    control_panel.right.keypad[2][2] = 9;
    control_panel.right.keypad[3][1] = 0;
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

void station_left_draw()
{
    uint16_t x = 16;
    uint16_t y = 152;

    station_left_t *station = &(control_panel.left);

    if (!control_panel.lights_off)
        rdp_draw_sprite_with_texture(labels[LABEL_RADIO], x, y - 20, 0);

    rdp_draw_filled_rectangle_size(x + 50, y - 20, 74, 14, colors[COLOR_BG]);
    rdp_draw_filled_rectangle_size(x + 52, y - 18, 70, 10, colors[COLOR_WHITE]);
    for (uint8_t i = 0; i < 6; i++)
        rdp_draw_filled_rectangle_size(x + 61 + i * 10, y - (i % 2 ? 16 : 14), 1, (i % 2 ? 6 : 4), colors[COLOR_BLACK]);

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
        rdp_draw_sprite_with_texture(labels[LABEL_AZ_5], x, y - 20, 0);
        rdp_draw_sprite_with_texture(tiles[(station->button_z ? 516 : 515)], x + 8, y - 2, 0);
    }
    x -= 4;
    y += 34;

    uint8_t cell_size = 9;
    uint8_t border = 2;

    rdp_draw_filled_rectangle_size(x, y, (MINIGRID_SIZE * cell_size) + ((MINIGRID_SIZE + 1) * border), (MINIGRID_SIZE * cell_size) + ((MINIGRID_SIZE + 1) * border), colors[COLOR_BG]);

    for (uint8_t yy = 0; yy < MINIGRID_SIZE; yy++)
    {
        for (uint8_t xx = 0; xx < MINIGRID_SIZE; xx++)
        {
            if ((xx + yy * MINIGRID_SIZE + 1 == station->joystick) || (xx + yy * MINIGRID_SIZE + 1 == 5 && station->joystick == 0))
                rdp_draw_filled_rectangle_size(x + border + (xx * (border + cell_size)), y + border + (yy * (border + cell_size)), cell_size, cell_size, colors[COLOR_YELLOW]);
        }
    }
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

    control_panel.freq = 180 + (79 * station->sliders[0]) + (-33 * station->sliders[1]) + (17 * station->sliders[2]) + (-7 * station->sliders[3]);

    if (input->Z)
    {
        station->button_z = !station->button_z;
        if (station->button_z)
            control_panel.stress = (control_panel.stress < 90) ? 90 : 100;
    }

    if (input->y > 90)
    {
        if (input->x < -90)
            station->joystick = 1;
        else if (input->x > 90)
            station->joystick = 3;
        else
            station->joystick = 2;
    }
    else if (input->y < -90)
    {
        if (input->x < -90)
            station->joystick = 7;
        else if (input->x > 90)
            station->joystick = 9;
        else
            station->joystick = 8;
    }
    else
    {
        if (input->x < -90)
            station->joystick = 4;
        else if (input->x > 90)
            station->joystick = 6;
    }
}

void station_center_draw()
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
        rdp_draw_sprite_with_texture(labels[LABEL_LIGHTS], x + 90, y, 0);
        rdp_draw_sprite_with_texture(tiles[502], x + 130, y + 15, (station->button_b ? MIRROR_Y : 0));

        // Button A
        rdp_draw_sprite_with_texture(labels[LABEL_PRESSURIZER], x + 100, y + 50, 0);
        rdp_draw_sprite_with_texture(tiles[(station->button_a ? 482 : 481)], x + 130, y + 65, 0);
        // TODO ADD BACK graphics_set_color(colors[COLOR_BLACK], 0);
        // TODO ADD BACK graphics_draw_textf(disp, x + 138, y + 71, "%d\n", station->button_a_presses);
        // TODO ADD BACK graphics_set_color(colors[COLOR_WHITE], 0);

        // Grid
        rdp_draw_sprite_with_texture(labels[LABEL_CONTROL_RODS], 10, 210, 0);
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

    control_panel.lights_off = station->button_b;
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

void station_right_draw_graphics(display_context_t disp)
{
    uint16_t x = 16;
    uint16_t y = 152;

    station_right_t *station = &(control_panel.right);

    x += 126;

    graphics_set_color(colors[COLOR_YELLOW], 0);
    if (station->cursor == 0)
    {
        if (control_panel.stress % 2 == 0)
            graphics_draw_textf_with_background(disp, x, y - 20, colors[COLOR_BLACK], "        ", station->screen);
        else
        {
            graphics_draw_textf_with_background(disp, x, y - 20, colors[COLOR_BLACK], "       _", station->screen);
        }
    }
    else
        graphics_draw_textf_with_background(disp, x, y - 20, colors[COLOR_BLACK], "% 8s", station->screen);
    graphics_set_color(colors[COLOR_WHITE], 0);
}

void station_right_draw()
{
    uint16_t x = 16;
    uint16_t y = 152;

    station_right_t *station = &(control_panel.right);

    if (!control_panel.lights_off)
    {
        rdp_draw_sprite_with_texture(labels[LABEL_TURBINES], 16, y - 20, 0);

        if (station->mode == MODE_LEVERS)
            rdp_draw_filled_rectangle_size(x + station->lever_selector * 20 + 8, y - 1, 15, 33, colors[COLOR_YELLOW]);

        rdp_draw_sprite_with_texture(tiles[500], x, y, station->levers[0] ? 0 : MIRROR_Y);
        rdp_draw_sprite_with_texture(tiles[504], x + 20, y, station->levers[1] ? 0 : MIRROR_Y);
        rdp_draw_sprite_with_texture(tiles[505], x + 40, y, station->levers[2] ? 0 : MIRROR_Y);
        rdp_draw_sprite_with_texture(tiles[506], x + 60, y, station->levers[3] ? 0 : MIRROR_Y);
    }

    if (!control_panel.lights_off)
    {
        rdp_draw_sprite_with_texture(labels[LABEL_PUMPS], 16, 190, 0);

        rdp_draw_filled_rectangle_size(16, 210, 118, 12, colors[COLOR_BG]);
        rdp_draw_filled_rectangle_size(18, 212, 114, 8, colors[COLOR_WHITE]);
    }
    uint8_t width = 2 + (station->rotations * 12);
    rdp_draw_filled_rectangle_size(20, 214, width, 4, colors[COLOR_RED]);

    x += 134;

    if (!control_panel.lights_off)
    {
        if (station->mode == MODE_KEYPAD)
            rdp_draw_filled_rectangle_size(x + station->keypadselector_x * 18, y + station->keypadselector_y * 18, 14, 15, colors[COLOR_YELLOW]);

        rdp_draw_sprite_with_texture(tiles[51], x, y, 0);
        rdp_draw_sprite_with_texture(tiles[52], x + 18, y, 0);
        rdp_draw_sprite_with_texture(tiles[53], x + 36, y, 0);
        rdp_draw_sprite_with_texture(tiles[54], x, y + 18, 0);
        rdp_draw_sprite_with_texture(tiles[55], x + 18, y + 18, 0);
        rdp_draw_sprite_with_texture(tiles[56], x + 36, y + 18, 0);
        rdp_draw_sprite_with_texture(tiles[57], x, y + 36, 0);
        rdp_draw_sprite_with_texture(tiles[58], x + 18, y + 36, 0);
        rdp_draw_sprite_with_texture(tiles[59], x + 36, y + 36, 0);
        rdp_draw_sprite_with_texture(tiles[293], x, y + 54, 0);
        rdp_draw_sprite_with_texture(tiles[60], x + 18, y + 54, 0);
        rdp_draw_sprite_with_texture(tiles[292], x + 36, y + 54, 0);
    }
}

void station_right_input(input_t *input)
{
    if (input->L)
        control_panel.current_station--;

    station_right_t *station = &(control_panel.right);

    if (station->mode == MODE_KEYPAD)
    {
        if (input->C_up)
            if (station->keypadselector_y > 0)
                station->keypadselector_y--;
        if (input->C_down)
            if (station->keypadselector_y < KEYPAD_H - 1)
                station->keypadselector_y++;
        if (input->C_left)
            if (station->keypadselector_x > 0)
                station->keypadselector_x--;
        if (input->C_right)
            if (station->keypadselector_x < KEYPAD_W - 1)
                station->keypadselector_x++;

        if (input->A)
        {
            if (station->keypadselector_x == 2 && station->keypadselector_y == 3)
            {
                station->validate = true;
            }
            else if (station->keypadselector_x == 0 && station->keypadselector_y == 3)
            {
                if (station->cursor != 0)
                {
                    station->cursor--;
                    station->screen[station->cursor] = 0;
                }
            }
            else
            {
                station->validate = false;
                if (station->cursor < CURSOR_MAX)
                {
                    station->screen[station->cursor] = '0' + station->keypad[station->keypadselector_y][station->keypadselector_x];
                    station->cursor++;
                }
            }
        }

        if (input->B)
        {
            memset(station->screen, 0, sizeof(station->screen));
            station->cursor = 0;
        }
    }
    else
    {
        if (input->C_left)
            if (station->lever_selector > 0)
                station->lever_selector--;
        if (input->C_right)
            if (station->lever_selector < NUM_LEVERS - 1)
                station->lever_selector++;

        if (input->A)
            station->levers[station->lever_selector] = !station->levers[station->lever_selector];
    }

    if (input->Z)
        station->mode = (station->mode == MODE_KEYPAD ? MODE_LEVERS : MODE_KEYPAD);

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