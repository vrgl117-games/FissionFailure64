#include <stdlib.h>

#include "actions.h"
#include "colors.h"
#include "control_panel.h"
#include "debug.h"
#include "fps.h"
#include "input.h"
#include "rdp.h"
#include "screens.h"

#define ENABLE_FPS 1

screen_t screen = game;

int main()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdp_init();
    rdp_set_default_clipping();
    control_panel_init();
    timer_init();
    debug_init_isviewer();
    actions_init();
    colors_init();

    control_panel_init();

    srand(timer_ticks() & 0x7FFFFFFF);

#if ENABLE_FPS
    // 1s
    new_timer(TIMER_TICKS(1000000), TF_CONTINUOUS, fps_timer);
#endif

    // 500ms
    new_timer(TIMER_TICKS(500000), TF_CONTINUOUS, control_panel_timer);

    // 20ms
    new_timer(TIMER_TICKS(20000), TF_CONTINUOUS, screen_timer);

    // 50ms
    new_timer(TIMER_TICKS(50000), TF_CONTINUOUS, input_timer);

    display_context_t disp = 0;

    while (true)
    {
        // get controllers
        input_t input = input_get();

        // wait for display
        while (!(disp = display_lock()))
            ;

        switch (screen)
        {
        case intro: // n64, n64brew jam and vrgl117 logo.
            if (screen_intro(disp))
                screen = vru;
            break;

        case vru:
            if (screen_vru(disp, &input))
                screen = game;
            break;
        case game: // actual game.
            screen = screen_game(disp, &input);
            break;
        case game_over:
            if (screen_game_over(disp, &input))
            {
                actions_reset();
                control_panel_reset();
                screen = game;
            }
            break;
        case win:
            if (screen_win(disp, &input))
            {
                actions_reset();
                control_panel_reset();
                screen = game;
            }
            break;
        }

#if ENABLE_FPS
        // increment fps counter
        fps_frame();

        // display fps
        fps_draw(disp);
#endif
        // update display
        display_show(disp);
    }

    return 0;
}
