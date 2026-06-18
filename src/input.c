#include "input.h"

static timer_link_t *presses_timer;
static volatile bool running = false;
static volatile uint8_t presses = 0;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void input_timer()
{
    presses = 0;
    running = false;
}

void input_init()
{
    joypad_init();
    presses_timer = new_timer(TIMER_TICKS(500000), TF_DISABLED, input_timer); //500ms
    running = false;
}

input_t input_get()
{
    joypad_poll();
    joypad_buttons_t buttons = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    joypad_inputs_t held = joypad_get_inputs(JOYPAD_PORT_1);
    input_t input = {0};
    input.A = buttons.a;
    input.B = buttons.b;
    input.Z = buttons.z;
    input.start = buttons.start;
    input.up = buttons.d_up;
    input.down = buttons.d_down;
    input.left = buttons.d_left;
    input.right = buttons.d_right;
    input.L = buttons.l;
    input.R = buttons.r;
    input.C_up = buttons.c_up;
    input.C_down = buttons.c_down;
    input.C_left = buttons.c_left;
    input.C_right = buttons.c_right;
    input.x = held.stick_x;
    input.y = held.stick_y;
    if (input.A)
    {
        if (!running)
        {
            restart_timer(presses_timer);
            running = true;
        }
        presses++;
    }
    return input;
}

uint8_t input_get_A_presses()
{
    return MIN(presses, 4);
}

void input_reset_presses()
{
    presses = 0;
}
