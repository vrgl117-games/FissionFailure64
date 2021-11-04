#include <stdlib.h>

#include "actions.h"
#include "colors.h"
#include "control_panel.h"
#include "debug.h"
#include "fps.h"
#include "input.h"
#include "rdp.h"
#include "screens.h"

#define ENABLE_FPS 0

#define MS500 500000
#define S1 1000000
#define MS20 20000
#define MS50 50000

screen_t screen = message;

int main()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdp_init();
    rdp_set_default_clipping();
    audio_init(44100, 4);
    mixer_init(16);
    control_panel_init();
    timer_init();
    debug_init_isviewer();
    actions_init();
    colors_init();

    control_panel_init();

    srand(timer_ticks() & 0x7FFFFFFF);

#if ENABLE_FPS
    new_timer(TIMER_TICKS(S1), TF_CONTINUOUS, fps_timer);
#endif

    // blinking "press start" and scientist position
    new_timer(TIMER_TICKS(MS20), TF_CONTINUOUS, screen_timer);

    // improve controller detection
    new_timer(TIMER_TICKS(MS50), TF_CONTINUOUS, input_timer);

    timer_link_t *game_timer = NULL;
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
                screen = message;
            //screen = vru;
            break;
        //case vru:
        //    if (screen_vru(disp, &input))
        //        screen = message;
        //    break;
        case message:
            if (screen_message(disp))
            {
                screen = title;
                wav64_t theme;
                wav64_open(&theme, "/sfx/01_Fission_Failure_64_Theme_mono.wav64");
                wav64_set_loop(&theme, true);
                mixer_ch_play(0, &theme.wave);
            }
            break;
        case title:
            if (screen_title(disp, &input))
            {
                screen = game;
                mixer_ch_stop(0);
                control_panel_reset();
                game_timer = new_timer(TIMER_TICKS(MS500), TF_CONTINUOUS, control_panel_timer);
            }
            break;
        case game: // actual game.
            screen = screen_game(disp, &input);
            if (screen != game && game_timer != NULL)
            {
                stop_timer(game_timer);
                game_timer = NULL;
            }
            break;
        case game_over:
            if (screen_game_over(disp, &input))
            {
                actions_reset();
                control_panel_reset();
                screen = title;
            }
            break;
        case win:
            if (screen_win(disp, &input))
            {
                actions_reset();
                control_panel_reset();
                screen = title;
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

        // Check whether one audio buffer is ready, otherwise wait for next frame to perform mixing.
        if (audio_can_write())
        {
            short *buf = audio_write_begin();
            mixer_poll(buf, audio_get_buffer_length());
            audio_write_end();
        }
    }

    return 0;
}
