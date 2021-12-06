#ifndef __CONTROL_PANEL_H__
#define __CONTROL_PANEL_H__

#include <stdint.h>

#include <libdragon.h>

#include "actions.h"
#include "input.h"

#define STRESS_THRESHOLD 280
#define HELL_THRESHOLD 620

typedef enum
{
    LABEL_LIGHTS,
    LABEL_RADIO,
    LABEL_TURBINES,
    LABEL_PUMPS,
    LABEL_CONTROL_RODS,
    LABEL_AZ_5,
    LABEL_PRESSURIZER,
    LABEL_DANGER,
    LABEL_WIND_TURBINES,

    TEXT_A,
    TEXT_B,
    TEXT_C,
    TEXT_D,
    TEXT_E,
    TEXT_F,
    TEXT_0,
    TEXT_1,
    TEXT_2,
    TEXT_3,
    TEXT_4,

    LABEL_IDX
} label_t;

#define NUM_STATIONS 3

#define NUM_SLIDERS 4
#define SLIDER_POSITONS 5
#define MINIGRID_SIZE 3
// hands on the left side (d-pad, joystick, Z and L)
typedef struct station_left
{
    uint8_t sliders[NUM_SLIDERS];
    uint8_t selected_slider;

    uint8_t compass;
    bool button_z;

} station_left_t;

#define GRID_SIZE_Y 4
#define GRID_SIZE_X 6
// hands on both sides (d-pad, A, B, C(s))
typedef struct station_center
{
    uint8_t grid[GRID_SIZE_Y][GRID_SIZE_X];
    uint8_t gridselector_x;
    uint8_t gridselector_y;
    bool button_a;
    bool button_b;
} station_center_t;

#define KEYPAD_H 4
#define KEYPAD_W 3
#define CURSOR_MAX 8
#define NUM_LEVERS 4
// hands on the right side (joystick, Z, A, B, C(s) and R)
typedef struct station_right
{
    uint8_t keypad[KEYPAD_H][KEYPAD_W];
    uint8_t keypadselector_x;
    uint8_t keypadselector_y;
    char screen[CURSOR_MAX + 1];
    uint8_t cursor;
    bool calling;

    bool levers[NUM_LEVERS];
    uint8_t lever_selector;

    enum
    {
        MODE_LEVERS,
        MODE_KEYPAD,
    } mode;

    uint8_t joystick;
    uint8_t rotations;

} station_right_t;

typedef enum control_panel_mode
{
    IDLE = 0,
    STRESSED = 1,
    HELL = 2,
} control_panel_mode_t;

typedef struct control_panel
{
    int8_t pressure;
    int16_t freq;
    int16_t power;
    uint16_t geiger; // 0 to 1000
    station_left_t left;
    station_center_t center;
    station_right_t right;

    control_panel_mode_t mode;
    uint8_t current_station;
    bool lights_off;
    uint8_t off_timer;
    uint16_t memory;
} control_panel_t;

typedef enum control_panel_status
{
    INCORRECT = 0,
    CORRECT = 1,
    DEAD = 2,
} control_panel_status_t;

control_panel_status_t control_panel_check_status(action_pair_t pair);
void control_panel_draw(display_context_t disp);
void control_panel_draw_tutorial(display_context_t disp);
void control_panel_init();
void control_panel_input(input_t *input, bool tutorial);
void control_panel_reset();
void control_panel_reset_tutorial();
void control_panel_timer();

#endif // __CONTROL_PANEL_H__