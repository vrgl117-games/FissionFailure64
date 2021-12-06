#ifndef __INPUT_H__
#define __INPUT_H__

#include <libdragon.h>

#define JOYSTICK_DEAD_ZONE 32

typedef struct SI_condat input_t;

input_t input_get();
void input_init();
void input_reset_presses();
uint8_t input_get_A_presses();

#endif //__INPUT_H__
