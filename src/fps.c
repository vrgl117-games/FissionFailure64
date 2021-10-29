#include "fps.h"

static volatile bool fps_refresh = false;
static volatile uint8_t fps;
static bool show = false;

extern uint32_t __width;

void fps_draw(display_context_t disp)
{
    if (show && fps > 0)
        debugf("FPS: %d\n", fps);
}

inline void fps_frame()
{
    static uint8_t frame_count = 0;

    frame_count++;
    if (fps_refresh)
    {
        fps = frame_count;
        frame_count = 0;
        fps_refresh = false;
    }
}

void fps_timer()
{
    fps_refresh = true;
}
