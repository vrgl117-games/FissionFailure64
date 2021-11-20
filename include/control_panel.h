#ifndef __CONTROL_PANEL_H__
#define __CONTROL_PANEL_H__

#include <stdint.h>

#include <libdragon.h>

#include "actions.h"
#include "input.h"

#define STRESS_THRESHOLD 28
#define HELL_THRESHOLD 62

#define NUM_STATIONS 3

#define NUM_SLIDERS 4
#define SLIDER_POSITONS 5
#define MINIGRID_SIZE 3
// hands on the left side (d-pad, joystick, Z and L)
typedef struct station_left
{
    uint8_t sliders[NUM_SLIDERS];
    uint8_t selected_slider;

    uint8_t joystick;
    bool button_z;

} station_left_t;

void station_left_draw(display_context_t disp);
void station_left_input(input_t *input);

#define GRID_SIZE 4
// hands on both sides (d-pad, A, B, C(s))
typedef struct station_center
{
    uint8_t grid[GRID_SIZE][GRID_SIZE];
    uint8_t gridselector_x;
    uint8_t gridselector_y;
    uint8_t button_a_presses;
    bool button_a;
    bool button_b;
} station_center_t;

void station_center_draw(display_context_t disp);
void station_center_input(input_t *input);

#define KEYPAD_H 4
#define KEYPAD_W 3
#define CURSOR_MAX 8

// hands on the right side (joystick, Z, A, B, C(s) and R)
typedef struct station_right
{
    uint8_t keypad[KEYPAD_H][KEYPAD_W];
    uint8_t keypadselector_x;
    uint8_t keypadselector_y;
    char screen[CURSOR_MAX + 1];
    uint8_t cursor;
    bool validate;

    uint8_t joystick;
    uint8_t rotations;

} station_right_t;

void station_right_draw(display_context_t disp);
void station_right_input(input_t *input);

typedef enum control_panel_mode
{
    IDLE = 0,
    STRESSED = 1,
    HELL = 2,
} control_panel_mode_t;

typedef struct control_panel
{
    int8_t temp;
    int16_t freq;
    int16_t power;
    u_int8_t stress; // 0 to 100
    station_left_t left;
    station_center_t center;
    station_right_t right;

    control_panel_mode_t mode;
    uint8_t current_station;
    bool lights_off;
} control_panel_t;

typedef enum control_panel_status
{
    INCORRECT = 0,
    CORRECT = 1,
    DEAD = 2,
} control_panel_status_t;

control_panel_status_t control_panel_check_status(action_t *action);
void control_panel_draw(display_context_t disp);
void control_panel_init();
void control_panel_input(input_t *input);
void control_panel_reset();
void control_panel_timer();

#endif // __CONTROL_PANEL_H__