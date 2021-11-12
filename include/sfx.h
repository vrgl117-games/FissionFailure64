#ifndef __SFX_H__
#define __SFX_H__

#include <libdragon.h>

#define FREQUENCY_44KHZ 44100

typedef enum sfx_id
{
    SFX_THEME = 0,
    SFX_IDLE,
    SFX_STRESS,
    SFX_HELL,
    SFX_BUTTON,
    SFX_ID_COUNT,
} sfx_id_t;

typedef enum ch_id
{
    CH_MUSIC = 0,
    CH_SFX,
    CH_ID_COUNT
} ch_id_t;

void sfx_init(void);
void sfx_play(const ch_id_t ch_id, const sfx_id_t sfx_id, bool loop);
void sfx_set_next_music(const sfx_id_t sfx_id);
void sfx_stop(const ch_id_t ch_id);
void sfx_update();

#endif //__SFX_H__
