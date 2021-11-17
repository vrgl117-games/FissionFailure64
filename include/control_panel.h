#ifndef __CONTROL_PANEL_H__
#define __CONTROL_PANEL_H__

#include <stdint.h>

#include <libdragon.h>

#include "actions.h"
#include "input.h"

#define STRESS_THRESHOLD 28
#define HELL_THRESHOLD 62

#define NUM_STATIONS 3

#define GRID_SIZE 4

// hands on the left side (d-pad, joystick, Z and L)
typedef struct station_left
{
    bool DPAD[4];
    bool Z;
    bool L;

} station_left_t;

void station_left_draw(display_context_t disp);
void station_left_input(input_t *input);

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

// hands on the right side (joystick, Z, A, B, C(s) and R)
typedef struct station_right
{
    bool Z;
    bool A;
    bool B;
    bool C[4];
    bool R;

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