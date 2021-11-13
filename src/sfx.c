#include "sfx.h"

static bool paused = false;
static wav64_t SFX_CACHE[SFX_ID_COUNT];
static sfx_id_t sfx_id_next_music = SFX_ID_COUNT;

volume_t volume_sfx = VOL_75;
volume_t volume_music = VOL_50;

void sfx_init(void)
{
    mixer_init(CH_ID_COUNT);

    wav64_open(&SFX_CACHE[SFX_THEME], "/sfx/01_Fission_Failure_64_Theme_mono.wav64");
    wav64_set_loop(&SFX_CACHE[SFX_THEME], true);

    wav64_open(&SFX_CACHE[SFX_IDLE], "/sfx/02_Fission_Failure_64_Idle_mono.wav64");
    wav64_set_loop(&SFX_CACHE[SFX_IDLE], true);

    wav64_open(&SFX_CACHE[SFX_STRESS], "/sfx/03_Fission_Failure_64_Stressed_mono.wav64");
    wav64_set_loop(&SFX_CACHE[SFX_STRESS], true);

    wav64_open(&SFX_CACHE[SFX_HELL], "/sfx/04_Fission_Failure_64_Hell_mono.wav64");
    wav64_set_loop(&SFX_CACHE[SFX_HELL], true);

    wav64_open(&SFX_CACHE[SFX_BUTTON], "/sfx/146718__leszek-szary__button.wav64");

    mixer_ch_set_vol(CH_MUSIC, (float)volume_music / 100.0f, (float)volume_music / 100.0f);
    mixer_ch_set_vol(CH_SFX, (float)volume_sfx / 100.0f, (float)volume_sfx / 100.0f);
}

void sfx_reset()
{
    sfx_id_next_music = SFX_ID_COUNT;
    paused = false;
}

void sfx_set_pause(const bool pause)
{
    paused = pause;
}

void sfx_play(const ch_id_t ch_id, const sfx_id_t sfx_id, bool loop)
{
    wav64_set_loop(&SFX_CACHE[sfx_id], loop);
    mixer_ch_play(ch_id, &SFX_CACHE[sfx_id].wave);
}

void sfx_set_next_music(const sfx_id_t sfx_id)
{
    if (sfx_id_next_music != sfx_id)
    {

        if (sfx_id_next_music != SFX_ID_COUNT)
        {
            // stop loop on current music
            mixer_ch_stop(CH_MUSIC);
            float pos = mixer_ch_get_pos(CH_MUSIC);
            wav64_set_loop(&SFX_CACHE[sfx_id_next_music], false);
            mixer_ch_play(CH_MUSIC, &SFX_CACHE[sfx_id_next_music].wave);
            mixer_ch_set_pos(CH_MUSIC, pos);
        }

        sfx_id_next_music = sfx_id;
    }
}

void sfx_stop(const ch_id_t ch_id)
{
    mixer_ch_stop(ch_id);
}

void sfx_switch_volume_music(bool left)
{
    switch (volume_music)
    {
    case VOL_100:
        volume_music = (left ? VOL_75 : VOL_0);
        break;
    case VOL_75:
        volume_music = (left ? VOL_50 : VOL_100);
        break;
    case VOL_50:
        volume_music = (left ? VOL_25 : VOL_75);
        break;
    case VOL_25:
        volume_music = (left ? VOL_0 : VOL_50);
        break;
    case VOL_0:
        volume_music = (left ? VOL_100 : VOL_25);
        break;
    }
    mixer_ch_set_vol(CH_MUSIC, (float)volume_music / 100.0f, (float)volume_music / 100.0f);
}

void sfx_switch_volume_sfx(bool left)
{
    switch (volume_sfx)
    {
    case VOL_100:
        volume_sfx = (left ? VOL_75 : VOL_0);
        break;
    case VOL_75:
        volume_sfx = (left ? VOL_50 : VOL_100);
        break;
    case VOL_50:
        volume_sfx = (left ? VOL_25 : VOL_75);
        break;
    case VOL_25:
        volume_sfx = (left ? VOL_0 : VOL_50);
        break;
    case VOL_0:
        volume_sfx = (left ? VOL_100 : VOL_25);
        break;
    }
    mixer_ch_set_vol(CH_SFX, (float)volume_sfx / 100.0f, (float)volume_sfx / 100.0f);
}

void sfx_update()
{
    if (paused)
        return;

    if (audio_can_write())
    {
        short *buf = audio_write_begin();
        mixer_poll(buf, audio_get_buffer_length());
        audio_write_end();
    }
    // current music stop, play next
    if (sfx_id_next_music != SFX_ID_COUNT && !mixer_ch_playing(CH_MUSIC))
    {
        sfx_play(CH_MUSIC, sfx_id_next_music, true);
    }
}