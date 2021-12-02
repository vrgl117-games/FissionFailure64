#include "input.h"

static timer_link_t *presses_timer;
static volatile uint8_t presses = 0;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void input_init()
{
    presses_timer = new_timer(TIMER_TICKS(500000), TF_ONE_SHOT, input_timer); //1s
}

input_t input_get()
{
    controller_scan();
    input_t input = get_keys_down().c[0];
    input_t pressed = get_keys_pressed().c[0];
    if (input.A)
    {
        if (presses == 0)
            restart_timer(presses_timer);
        presses++;
    }
    input.x = pressed.x;
    input.y = pressed.y;

#if DISPLAY_SAFE_AREAS
    if (pressed.L)
    {
        input.L = true;
    }
    if (pressed.R)
    {
        input.R = true;
    }
#endif
    return input;
}

uint8_t input_get_A_presses()
{
    return MIN(presses, 4);
}

void input_timer()
{
    presses = 0;
}