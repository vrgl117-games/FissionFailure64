#include "input.h"

static volatile int tick = 0;

input_t input_get()
{
    controller_scan();
    input_t input = get_keys_down().c[0];
    if (input.up || input.down || input.left || input.right)
        tick = 0;

    input_t pressed = get_keys_pressed().c[0];

    if (pressed.up && tick == 3)
    {
        input.up = true;
        tick = 1;
    }
    if (pressed.down && tick == 3)
    {
        input.down = true;
        tick = 1;
    }
    if (pressed.left && tick == 3)
    {
        input.left = true;
        tick = 1;
    }
    if (pressed.right && tick == 3)
    {
        input.right = true;
        tick = 1;
    }
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
/*
void input_vru()
{
    if (identify_accessory(3) == ACCESSORY_VRU)
    {
        uint8_t out[64] = {0};
        uint8_t in[64] = {0};
        execute_raw_command(3, VRU_READ, 0, 34, out, in);
        for (int i = 0; i < 34; i++)
            debugf("%02X", in[i]);
        debugf("\n");
    }
}
*/
void input_timer()
{
    tick++;
}